
CONFIG(debug, debug|release): {
LIBS += -L$$PWD/QRencodeLib/lib/ -lqrencoded
}else{
LIBS += -L$$PWD/QRencodeLib/lib/ -lqrencode
}
INCLUDEPATH += $$PWD/QRencodeLib/include
DEPENDPATH += $$PWD/QRencodeLib/include
