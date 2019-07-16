#ifndef contactlistenerH
#define contactlistenerH

class MContactListener: public b2ContactListener
{
private:
	void BeginContact(b2Contact* pContact);
	void EndContact(b2Contact* pContact);
};

#endif
