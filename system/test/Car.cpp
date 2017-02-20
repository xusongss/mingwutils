#define LOG_TAG "car"
#include <stddef.h>
#include<options/Option.h>
#include<utils/Log.h>
struct Ctx{
	Option ctx;
	int mWidth;
	int mHight;
};
class Car {
public:
	Car();
	virtual ~Car();

public:
	void display();
public:
	Option *getCtx();

private:
	Ctx mCtx;
};
/**
 * ---------------------------------------------------------------------------------------------
 */
static Instruction ops[] = {
	{ "width", "set car width", offsetof(Ctx, mWidth), OPT_TYPE_INT , {.i64=3}, 0, 100},
	{ "hight", "set car hight", offsetof(Ctx, mHight), OPT_TYPE_INT , {.i64=4}, 0, 100},
	{NULL}
};
Car::Car() {
	mCtx.ctx.ops = ops;
}
Car::~Car() {
}
Option *Car::getCtx(){
	return &mCtx.ctx;
}
void Car::display(){
	ALOGV("width %d", this->mCtx.mWidth);
	ALOGV("hight %d", this->mCtx.mHight);

}
int main() {
	Car car;
	optionSet(car.getCtx(),"width", "2");
	optionSet(car.getCtx(),"hight", "3");
	car.display();
	optionSet(car.getCtx(),"width", "3");
	optionSet(car.getCtx(),"hight", "4");
	car.display();
	optionSet(car.getCtx(),"width", "5");
	optionSet(car.getCtx(),"hight", "6");
	car.display();
}


