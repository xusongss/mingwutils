#define LOG_TAG "car"
#include <stddef.h>

#include<utils/Log.h>
#include <stdio.h>
#include "Car.h"
using namespace Athena;
static void Notify(config_ctx_t *ctx, const char * key, const char * val);
struct Ctx{
	config_ctx_t ctx;//must the first
	int mWidth;
	int mHight;
	int mKg;
	Car *mParent;
};
/**
 * ---------------------------------------------------------------------------------------------
 */
static Option_t ops[] = {
		{ "width", "set car width", offsetof(Ctx, mWidth), 	OPT_TYPE_INT , {.i64=100}, 0, 1000, 0, 0, .notify=(notify_f)Notify},
		{ "hight", "set car hight", offsetof(Ctx, mHight), 	OPT_TYPE_INT , {.i64=200}, 0, 2000, 0, 0,	.notify=(notify_f)Notify},
		{ "kg", 	"set car hight", offsetof(Ctx, mKg), 		OPT_TYPE_INT , {.i64=300}, 0, 3000, 0, 0,	.notify=(notify_f)Notify},
		{NULL}
};
static void Notify(config_ctx_t *ctx, const char * key, const char * val){
	ALOGV("Notify %s set key %s val %s", ((Ctx*)ctx)->mParent->getName(),key, val);
}

Car::Car(const char * name) {
    Ctx *ctx = new Ctx;
    ctx->mParent = this;
    ctx->ctx.ops = ops;
    mCtx = (void*)ctx;
	sprintf(this->mName, name);
	this->setDefault();
}
Car::~Car() {
}
config_ctx_t *Car::getCtx(){
    Ctx *ctx = ( Ctx *)mCtx;
	return &ctx->ctx;
}
int Car::setParameter(const char * key, const char * val){
	this->set(key, val);
}
void Car::display(){
    Ctx *ctx = ( Ctx *)mCtx;
	ALOGV("  width %d", ctx->mWidth);
	ALOGV("  hight %d", ctx->mHight);
	ALOGV("  kg %d", ctx->mKg);

}
int main() {
	Car car("Audi");
	int i = 0;
	ALOGV("default:");
	car.display();

	car.setParameter("width", "2");
	car.setParameter("hight", "3");
	car.setParameter("kg", "4");
	car.display();

	car.setParameter("width", "12");
	car.setParameter("hight", "13");
	car.setParameter("kg", "14");
	car.display();
	car.setParameter("width", "22");
	car.setParameter("hight", "23");
	car.setParameter("kg", "24");
	car.display();

	car.setParameter("width", "32");
	car.setParameter("hight", "33");
	car.setParameter("kg", "34");
	car.display();

	car.setParameter("width", "132");
	car.setParameter("hight", "133");
	car.setParameter("kg", "134");
	car.display();
	car.setParameter("width", "1032");
	car.setParameter("hight", "1033");
	car.setParameter("kg", "1034");
	car.display();
	car.setParameter("width", "2032");
	car.setParameter("hight", "2033");
	car.setParameter("kg", "2034");
	car.display();
	car.setParameter("width", "3032");
	car.setParameter("hight", "3033");
	car.setParameter("kg", "3034");
	car.display();
}


