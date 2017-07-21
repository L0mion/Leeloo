#ifndef LEELOO_UTILITY_H
#define LEELOO_UTILITY_H

#define SAFE_RELEASE(com) if(com != nullptr){com->Release(); com = nullptr;}
#define SAFE_DELETE(obj) if(obj != nullptr){delete obj; obj = nullptr;}
#define SAFE_DELETE_ARRAY(arr) if(arr != nullptr){delete arr[]; arr = nullptr;}

#endif //LEELOO_UTILITY_H