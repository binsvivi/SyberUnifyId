#ifndef ANDROID_CONFIGMANAGER_H
#define ANDROID_CONFIGMANAGER_H

//#include <unifyid/IUnifyIdManager.h>
//#include <utils/Functor.h>
//#include <binder/BinderService.h>
//#include <cutils/compiler.h>
//#include <utils/Vector.h>
//#include <utils/Mutex.h>
//#include <utils/KeyedVector.h>
//#include <utils/RWLock.h>
//#include <utils/StrongPointer.h>
#include <sys/types.h>
//#include <regex.h>
#include <string>
namespace android {

    class UnifyIdManager
 //           public BinderService<UnifyIdManager> ,
 //           public BnUnifyIdManager,
 //           public IBinder::DeathRecipient
    {
    public:
        UnifyIdManager();
        ~UnifyIdManager();
//        static char const * getServiceName() ANDROID_API { return "unifyIdManager"; }
        void featSlide(std::string samplePath, std::string featPath);
	double testSlide(std::string modulePath, std::string featPath);
	void trainSVM(std::string featPath, std::string savePath);
        void featGesture(std::string samplePath, std::string featPath);
	double testGesture(std::string modulePath, std::string featPath);
	void trainGesture(std::string featPath, std::string savePath);
        //void binderDied(const wp<IBinder>& who);


    private:
        enum {
            NOERROR = 0
        };
        enum {
            GETERROR_INEXISTENCE = -1,
            GETERROR_INVALID = -2,
            GETERROR_NOKNOWN = -3
        };
        enum {
            SETERROR_IINVALID = -1,
            SETERROR_NOKNOWN = -2
        };
    private:
        bool isValidKey(const std::string key) const;
        bool setToCache(const std::string &key, const std::string &value);
        bool getFromCache(const std::string &key, std::string &value);
        bool isInCache(const std::string &key);
        bool initCache();
        bool isReserve(const std::string &key);
   // private:
        //Mutex mRegistrationLock;

       // RWLock mCacheLock;
       // KeyedVector<String8, String8> mCache;

       // sp<ssalConfigWrapper> ssalWrapper;

       // regex_t re;
       // Functor *mNotifyFunctor;
    };

};

#endif //ANDROID_CONFIGMANAGER_H
