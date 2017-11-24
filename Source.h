#ifndef _SOURCE_H
#define _SOURCE_H

class Source {
public:
	Source();
	
	virtual Vect getLightPosition() { return Vect(0, 0, 0); }
	virtual CSColor getColor() { return CSColor(1, 1, 1, 0); }
};

Source::Source() {}

#endif