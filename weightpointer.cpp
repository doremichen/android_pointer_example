/*
 * Command line access to Android pointer example.
 *
 */
#include <stdio.h>
#include <utils/RefBase.h>

#define INITIAL_STRONG_VALUE (1<<28)

using namespace android;

//base class
class WeightClass : public RefBase
{

public:
    void printRefCount()
    {
        int32_t strong = getStrongCount();
        weakref_type *ref = getWeakRefs();
        
        printf("------------------------------------------------------\n");
        printf("[%s]: Strong Ref Count[%d].\n", __FUNCTION__, (strong == INITIAL_STRONG_VALUE ? 0 : strong));
        printf("[%s]: Weak Ref Count[%d].\n", __FUNCTION__, ref->getWeakCount());
        printf("------------------------------------------------------\n");    
        
    }        
};         

//Strong class
class StrongClass : public WeightClass
{

public:
    StrongClass()
    {
        printf("[%s]: default constructor\n", __FUNCTION__);  
    }
    
    virtual ~StrongClass()
    {
        printf("[%s]: destructor\n", __FUNCTION__);    
    }        
        
};


//Weak class
class WeakClass : public WeightClass
{
    
public:
    WeakClass()
    {
        printf("[%s]: default constructor\n", __FUNCTION__);
        
        extendObjectLifetime(OBJECT_LIFETIME_WEAK);     
    }
    
    virtual ~WeakClass()
    {
    
        printf("[%s]: destructor\n", __FUNCTION__);
        
    }                
    
    
};


//Forever class
//class ForeverClass : public WeightClass
//{
//
//public:
//    ForeverClass()
//    {
//        printf("[%s]: default constructor\n", __FUNCTION__);
//        
//        extendObjectLifetime(OBJECT_LIFETIME_FOREVER);     
//    }
//    
//    virtual ~ForeverClass()
//    {
//    
//        printf("[%s]: destructor\n", __FUNCTION__);
//        
//    }                
//        
//};

//Test Strong class
void TestStrongClass(StrongClass *pStrongClass)
{
    wp<StrongClass> wpOut = pStrongClass;
    pStrongClass -> printRefCount();
    
    {
        sp<StrongClass> spInner = pStrongClass;
        pStrongClass -> printRefCount();   
        
    }
    
    sp<StrongClass> spOut = wpOut.promote();    
    
    printf("[%s]: spOut[%p].\n", __FUNCTION__, spOut.get());
    
}

//Test Weak class
void TestWeakClass(WeakClass *pWeakClass)
{
    wp<WeakClass> wpOut = pWeakClass;
    pWeakClass -> printRefCount();
    
    {
        sp<WeakClass> spInner = pWeakClass;
        pWeakClass -> printRefCount();   
        
    }
    
    pWeakClass -> printRefCount();
    sp<WeakClass> spOut = wpOut.promote();    
    
    printf("[%s]: spOut[%p].\n", __FUNCTION__, spOut.get());
    
}


//Test forever class
//void TestForeverClass(ForeverClass *pForeverClass)
//{
//    wp<ForeverClass> wpOut = pForeverClass;
//    pForeverClass -> printRefCount();
//    
//    {
//        sp<ForeverClass> spInner = pForeverClass;
//        pForeverClass -> printRefCount();   
//        
//    }
//        
//}


int main(int argc, char **argv)
{
    printf("[%s]: test strong class\n", __FUNCTION__);
    StrongClass *pStrongClass =  new StrongClass();
    TestStrongClass(pStrongClass);
    
    printf("[%s]: test weak class\n", __FUNCTION__);
    WeakClass *pWeakClass =  new WeakClass();
    TestWeakClass(pWeakClass);
    
    return 0;    
}    