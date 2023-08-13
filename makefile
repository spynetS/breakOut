run:
	rm -rf ./a.out
	gcc ./m.c ../sound/sound.c ../printer/src/Canvas.c ../printer/src/msc.c ./main.c -lm -lasound
	./a.out
