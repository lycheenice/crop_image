#! /bin/bash
./djpegb raw "$1".jpg -raw_out
./rgb2grey "$1".raw "$1".gry 640 480
