CC      = g++
SRCDIR  = ./source
OBJDIR	= ./obj

all: clean setupdir tsp 

tsp: utilities.o opt.o nearest_neighbour.o ant.o antcolony.o main.o 
	cd $(OBJDIR) ; $(CC) -o ../TSP $^ 

utilities.o: $(SRCDIR)/utilities.cpp
	$(CC) -c $< -o $(OBJDIR)/$@

opt.o: $(SRCDIR)/opt.cpp
	$(CC) -c $< -o $(OBJDIR)/$@

nearest_neighbour.o: $(SRCDIR)/nearest_neighbour.cpp
	$(CC) -c $< -o $(OBJDIR)/$@

ant.o: $(SRCDIR)/ant.cpp
	$(CC) -c $< -o $(OBJDIR)/$@

antcolony.o: $(SRCDIR)/antcolony.cpp
	$(CC) -c $< -o $(OBJDIR)/$@

main.o: $(SRCDIR)/main.cpp
	$(CC) -c $< -o $(OBJDIR)/$@

setupdir: 
	mkdir $(OBJDIR)

clean:
	rm -fR $(OBJDIR) tsp
