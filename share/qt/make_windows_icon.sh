#!/bin/bash
# create multiresolution windows icon
ICON_DST=../../src/qt/res/icons/InterstellarHoldings.ico

convert ../../src/qt/res/icons/InterstellarHoldings-16.png ../../src/qt/res/icons/InterstellarHoldings-32.png ../../src/qt/res/icons/InterstellarHoldings-48.png ${ICON_DST}
