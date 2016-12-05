objects = cluster.o clustercommand.o counttable.o groupmap.o listvector.o mothur.o mothurout.o nameassignment.o optionparser.o ordervector.o progress.o rabundvector.o readcolumn.o sabundvector.o sequence.o sharedordervector.o sharedrabundvector.o sharedsabundvector.o sharedutilities.o sparsedistancematrix.o uvest.o validparameter.o 

all: $(objects)
		nvcc -arch=sm_20 $(objects) -o app

%.o: %.cpp
		nvcc -x cu -arch=sm_20 -I. -dc $< -o $@

clean:
		rm -f *.o app