cd ./3dparty/Scintilla/Qt4Qt5 || exit
qmake
make
cd ../../..

cmake . -Bbuild -Wdev -Werror=dev && cmake --build ./build