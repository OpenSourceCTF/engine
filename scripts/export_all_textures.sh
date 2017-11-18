#!/bin/sh

cd tp_textures || exit
for dir in */; do
  dir=$(echo "${dir}" | sed 's:/::g')

  #boosts
  mkdir ttmp
  convert "${dir}/speedpad.png" -crop 40x40+0+0 ttmp/1.png
  convert "${dir}/speedpadblue.png" -crop 40x40+0+0 ttmp/2.png
  convert "${dir}/speedpadred.png" -crop 40x40+0+0 ttmp/3.png

  #portals
  convert "${dir}/portal.png" -crop 40x40+0+0 ttmp/5.png

  #spikes
  convert "${dir}/tiles.png" -crop 40x40+480+0 ttmp/6.png

  #balls
  convert "${dir}/tiles.png" -crop 40x40+560+0 ttmp/7.png
  convert "${dir}/tiles.png" -crop 40x40+600+0 ttmp/8.png

  #bombs
  convert "${dir}/tiles.png" -crop 40x40+480+40 ttmp/9.png
  convert "${dir}/tiles.png" -crop 40x40+480+80 ttmp/10.png

  #flags
  convert "${dir}/tiles.png" -crop 40x40+520+40 ttmp/11.png
  convert "${dir}/tiles.png" -crop 40x40+520+80 ttmp/12.png
  convert "${dir}/tiles.png" -crop 40x40+560+40 ttmp/13.png
  convert "${dir}/tiles.png" -crop 40x40+560+80 ttmp/14.png
  convert "${dir}/tiles.png" -crop 40x40+600+40 ttmp/15.png
  convert "${dir}/tiles.png" -crop 40x40+600+80 ttmp/16.png

  #pups
  convert "${dir}/tiles.png" -crop 40x40+480+160 ttmp/17.png
  convert "${dir}/tiles.png" -crop 40x40+480+200 ttmp/18.png
  convert "${dir}/tiles.png" -crop 40x40+480+240 ttmp/19.png

  #toggle
  convert "${dir}/tiles.png" -crop 40x40+520+240 ttmp/20.png

  #get color and fill rectangle
  p2rect () {
    a=$(convert "${dir}/tiles.png[1x1+${1}+${2}]"\
        -format "%[fx:int(255*r)],%[fx:int(255*g)],%[fx:int(255*b)]"\
        info:)
    convert -size 40x40 xc:transparent\
            -fill "rgba(${a},${3})"\
            -draw "rectangle 0,0 40,40"\
            "ttmp/${4}.png"
  }

  #gates
  p2rect 480 120 0.6 21
  p2rect 520 120 0.6 22
  p2rect 560 120 0.6 23
  p2rect 600 120 0.6 24

  #walls
  p2rect 10 10 1 25

  #tiles
  p2rect 540 168 1 26

  if [ ! -d "../textures/${dir}" ]; then
    mkdir "../textures/${dir}"
  fi

  montage ttmp/*.png -background none -geometry +0+0 "../textures/${dir}/tiles.png"
  rm -rf ttmp
done
