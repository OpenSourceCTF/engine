#include <stack>
#include "util.hpp"

float map_val(
    const float v,
    const float ax,
    const float ay,
    const float bx,
    const float by
) {
      return bx + (by - bx) * ((v - ax) / (ay - ax));
}

float dist(
    const float x1,
    const float y1,
    const float x2,
    const float y2
) {
    return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

// converts [-1, 0, 1] to angle in radians
float angle_from_input(const int x, const int y)
{
    std::uint32_t m = 0;

    if(x) m = 4 + ((x + 1) << 1);
    if(y) m = 6 + ((y + 1) << 1);
    if(x && y) m += x == y ? -1 : 1;

    return TWO_PI / 8 * (m & 0x7);
}

std::vector<std::string> split_on(const std::string & str, const char n)
{
    std::vector<std::string> pieces;
    std::stringstream ss(str);
    while(ss.good()) {
        std::string sub;
        std::getline(ss, sub, n);
        pieces.emplace_back(sub);
    }

    return pieces;
}

/* returns a container of containers of vertices
 * which define the same regions as the input set
 * of polygons
 */
std::vector<std::unique_ptr<chain>> poly2chain(std::vector<polygon> poly_set) {

    using edge = std::pair<std::pair<float,float>,std::pair<float,float>>;

    // TODO: use a quicker hash
    auto sort_poly = [](const polygon& poly) {
        std::vector<std::pair<float,float>> sorted;
        sorted.push_back(std::make_pair(poly.x1,poly.y1));
        sorted.push_back(std::make_pair(poly.x2,poly.y2));
        sorted.push_back(std::make_pair(poly.x3,poly.y3));
        std::sort(sorted.begin(),sorted.end());
        return sorted;
    };

    auto poly_hash = [&sort_poly](const polygon& poly) {
        auto sorted = sort_poly(poly);
        std::string s = std::to_string(sorted[0].first)
                      + std::to_string(sorted[0].second)
                      + std::to_string(sorted[1].first)
                      + std::to_string(sorted[1].second)
                      + std::to_string(sorted[2].first)
                      + std::to_string(sorted[2].second);
        return std::hash<std::string>()(s);
    };

    auto poly_equal = [&sort_poly](const polygon& A, const polygon& B) {
        auto A_sorted = sort_poly(A);
        auto B_sorted = sort_poly(B);

        return    (A_sorted[0].first == B_sorted[0].first
                   && A_sorted[0].second == B_sorted[0].second)
               && (A_sorted[1].first == B_sorted[1].first
                   && A_sorted[1].second == B_sorted[1].second)
               && (A_sorted[2].first == B_sorted[2].first
                   && A_sorted[2].second == B_sorted[2].second);
    };

    auto edge_hash = [](const edge& e) {
        std::string s = std::to_string(e.first.first)
                      + std::to_string(e.first.second)
                      + std::to_string(e.second.first)
                      + std::to_string(e.second.second);
        return std::hash<std::string>()(s);
    };

    auto edge_lt = [](const edge& A, const edge& B) {
        std::vector<std::pair<float,float>> A_sorted({A.first,A.second});
        std::vector<std::pair<float,float>> B_sorted({B.first,B.second});
        std::sort(A_sorted.begin(),A_sorted.end());
        std::sort(B_sorted.begin(),B_sorted.end());
        return A_sorted < B_sorted;
    };

    auto edge_equal = [&edge_lt](const edge& A, const edge& B) {
        return ((!edge_lt(A,B)) && (!edge_lt(B,A)));
    };

    auto point_hash = [](const std::pair<float,float>& A) {
        std::string s = std::to_string(A.first)
                      + std::to_string(A.second);
        return std::hash<std::string>()(s);
    };

    // ======= actual algorithm begins here =======

    std::unordered_set<polygon,
                       decltype(poly_hash),
                       decltype(poly_equal)> seen(poly_set.size() * 2,poly_hash,poly_equal);

    std::map<edge,int,decltype(edge_lt)> counter(edge_lt);

    for(auto const& p : poly_set) {
        //ignore overlapping polys
        if(seen.find(p) == seen.end()) {
            seen.insert(p);

            edge edge1 = std::make_pair(std::make_pair(p.x1,p.y1),
                                        std::make_pair(p.x2,p.y2));
            edge edge2 = std::make_pair(std::make_pair(p.x2,p.y2),
                                        std::make_pair(p.x3,p.y3));
            edge edge3 = std::make_pair(std::make_pair(p.x3,p.y3),
                                        std::make_pair(p.x1,p.y1));
            ++counter[edge1];
            ++counter[edge2];
            ++counter[edge3];
        }
    }

    std::unordered_set<edge,
                       decltype(edge_hash),
                       decltype(edge_equal)> outside_edges(poly_set.size() * 2,
                                                           edge_hash,
                                                           edge_equal);

    /* get the good edges and keep track
     * of points that are on one in a sort of
     * adjacency list
     */
    std::map<std::pair<float,float>,std::vector<edge>> e_firsts;
    std::map<std::pair<float,float>,std::vector<edge>> e_seconds;
    for(auto const& e : counter) {
        if(counter.at(e.first) == 1) {
            outside_edges.insert(e.first);
            e_firsts[e.first.first].push_back(e.first);
            e_seconds[e.first.second].push_back(e.first);
        }
    }

    if(outside_edges.begin() == outside_edges.end()) {
        //shouldn't happen unless the polygons live on a torus
        std::cerr << "something is really wrong" << std::endl;
        exit(EXIT_FAILURE);
    } 

    std::unordered_set<edge,
                       decltype(edge_hash)> visited_e(outside_edges.size() * 2,
                                                      edge_hash);
    std::unordered_set<std::pair<float,float>,
                       decltype(point_hash)> sing_points(outside_edges.size()
                                                         * 4,
                                                         point_hash);
    std::vector<std::unique_ptr<chain>> result;

    //collect the good edges into cycles
    //it'd be more efficient to erase edges as we go along
    std::stack<std::pair<float,float>> leftovers;
    for(auto i = outside_edges.begin();
             i != outside_edges.end()
               && visited_e.size() != outside_edges.size();
             ++i)
    {
        if(visited_e.find(*i) == visited_e.end()) {
            result.emplace_back();
            std::unique_ptr<chain>& CC = result.back();
            auto first_e = *i;

            auto current_p = first_e.first;
            CC->vertices.emplace_back();
            coord c = coord(current_p.first, current_p.second);
            CC->vertices.back().reset(&c);
            bool CC_end = false;

            //for safety
            int maxit = poly_set.size() * 3;
            int iter = 0;
            while(!CC_end && iter < maxit) {
                auto explore1 = e_firsts.find(current_p);
                auto explore2 = e_seconds.find(current_p);
                std::pair<float,float> new_p(std::make_pair(-1,-1));
                bool found = false;
                //stop when touching corners ><
                bool sing = (  (explore1 != e_firsts.end()
                               ? explore1->second.size()
                               : 0) 
                             + (explore2 != e_seconds.end()
                               ? explore2->second.size()
                               : 0)
                             >= 4);
                if(sing) sing_points.insert(current_p);
                if(explore1 != e_firsts.end()) {
                    for(auto it=explore1->second.begin();
                             it!=explore1->second.end() && !found;
                             ++it)
                    {
                        if(   visited_e.find(*it) == visited_e.end()
                           && *it != first_e)
                        {
                            new_p = it->second;
                            if(new_p != current_p) {
                                visited_e.insert(*it);
                                found = true;
                            }
                        }
                    }
                }
                if(explore2 != e_seconds.end()) {
                    for(auto it=explore2->second.begin();
                             it!=explore2->second.end() && !found;
                             ++it)
                    {
                        if(visited_e.find(*it) == visited_e.end()) {
                            new_p = it->first;
                            if(new_p != current_p) {
                                visited_e.insert(*it);
                                found = true;
                            }
                        }
                    }
                }
                if(   new_p == std::make_pair<float,float>(-1,-1)) {
                    CC_end = true;
                }
                else if (sing_points.find(new_p) != sing_points.end()){
                    CC_end = true;
                    CC->vertices.emplace_back();
                    coord c = coord(current_p.first, current_p.second);
                    CC->vertices.back().reset(&c);
                    leftovers.push(first_e.second);
                }
                else {
                    current_p = new_p;
                    CC->vertices.emplace_back();
                    coord c = coord(current_p.first, current_p.second);
                    CC->vertices.back().reset(&c);
                }
                ++iter;
            }
        }
    }
    while(!leftovers.empty()) {
        auto current_p = leftovers.top(); leftovers.pop();
        auto explore2 = e_seconds.find(current_p);
        auto explore1 = e_firsts.find(current_p);
        std::pair<float,float> new_p;
        if(explore1 != e_firsts.end()) {
            for(auto it=explore1->second.begin();
                     it!=explore1->second.end();
                     ++it)
            {
                new_p = it->second;
                if(new_p != current_p) {
                    result.emplace_back();
                    std::unique_ptr<chain>& CC = result.back();
                    CC->vertices.emplace_back();
                    coord c = coord(current_p.first, current_p.second);
                    CC->vertices.back().reset(&c);
                }
            }
        }
        if(explore2 != e_seconds.end()) {
            for(auto it=explore2->second.begin();
                     it!=explore2->second.end();
                     ++it)
            {
                new_p = it->first;
                if(new_p != current_p) {
                    result.emplace_back();
                    std::unique_ptr<chain>& CC = result.back();
                    CC->vertices.emplace_back();
                    coord c = coord(current_p.first, current_p.second);
                    CC->vertices.back().reset(&c);
                }
            }
        }
    }
    return result;
}
