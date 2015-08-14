#!/bin/bash
./makesrcpackage.sh
cd panicbutton-0.5.0
mad qmake PREFIX=/usr panicbutton.pro
mad make
mad make -n install

