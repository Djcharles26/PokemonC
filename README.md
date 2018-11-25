pokémon C

This is a version of pokémon (C) version in pure C and Allegro 5


Installation:

If you are in Ubuntu Distro, you should install allegro, to do that you must

1.Open a Terminal (ctrl + alt + T)

2. sudo apt update

3. apt-get install build-essential git cmake cmake-curses-gui xorg-dev libgl1-mesa-dev libglu-dev

Optional Dependecies

4. apt-get install libpng-dev libcurl4-openssl-dev libfreetype6-dev libjpeg-dev libvorbis-dev libopenal-dev libphysfs-dev \
 libgtk2.0-dev libasound-dev libpulse-dev libflac-dev libdumb1-dev
 
 Documentation Dependencies 
 
 5.apt-get install exuberant-ctags dvi2ps dvipdfmx latex2html pandoc
 
 Getting allegro
 
 6. git clone https://github.com/liballeg/allegro5.git
 
 7.cd allegro
 
 8. git checkout 5.2
 
 9. mkdir build
 
 10. cd build
 
 11 .ccmake -DCMAKE_INSTALL_PREFIX=/usr ..
 
 12. make
 
 If you have an ice cream distcc you should 
 
 make -j8
 
 13. make install
 
 And you have it now!
 
 For Windows 

In MINGW and Codeblocks

1. http://liballeg.org/download.html#windows download Dependencies from here

Follow steps in : https://wiki.allegro.cc/index.php?title=Windows,_Code::Blocks_and_Allegro_5




