CC = gcc
CFLAGS = -g -std=c17 -Wall
LDLIBS = -lm -lrt -lMLV
TOUT = bin/gestion_gemme.o bin/main.o bin/generation_terrain.o bin/gestion_monstre.o bin/gestion_tirs.o bin/gestion_tour.o bin/graphique.o bin/gestion_mana.o bin/moteur.o

jeu: $(TOUT)
	$(CC) $(CFLAGS) -o jeu $(TOUT) $(LDLIBS)

bin/gestion_gemme.o: src/gestion_gemme.c include/gestion_gemme.h
	$(CC) $(CFLAGS) -c -o $@ $<

bin/gestion_mana.o: src/gestion_mana.c include/gestion_mana.h
	$(CC) $(CFLAGS) -c -o $@ $<

bin/gestion_monstre.o: src/gestion_monstre.c include/gestion_monstre.h
	$(CC) $(CFLAGS) -c -o $@ $<

bin/generation_terrain.o: src/generation_terrain.c include/generation_terrain.h
	$(CC) $(CFLAGS) -c -o $@ $<

bin/gestion_tirs.o: src/gestion_tirs.c include/gestion_tirs.h
	$(CC) $(CFLAGS) -c -o $@ $<

bin/gestion_tour.o: src/gestion_tour.c include/gestion_tour.h
	$(CC) $(CFLAGS) -c -o $@ $<

bin/graphique.o: src/graphique.c include/graphique.h
	$(CC) $(CFLAGS) -c -o $@ $<

bin/moteur.o: src/moteur.c include/moteur.h
	$(CC) $(CFLAGS) -c -o $@ $<

bin/main.o: src/main.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -rf $(TOUT) jeu
