g++  -Wall $(root-config --cflags --libs) -o Analysis src/Analysis.C
g++  -Wall $(root-config --cflags --libs) -o Waveforms src/Waveforms.C
g++  -Wall $(root-config --cflags --libs) -o Average src/Average.C
