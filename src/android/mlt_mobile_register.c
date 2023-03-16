//
// Created by apple on 2022/11/21.
//
#include "mlt_mobile_register.h"
extern void mlt_register_core(mlt_repository repository);
#ifdef OPTION_FFMPEG
extern void mlt_register_ffmpeg(mlt_repository repository);
#endif
extern void (* const mlt_mobile_register[])(mlt_repository repository);
void (* const mlt_mobile_register[])(mlt_repository repository) = {
        mlt_register_core,
#ifdef OPTION_FFMPEG
        mlt_register_ffmpeg,
#endif
};

void mlt_register_mobile(mlt_repository repository)
{
    for (int i = 0; i < (sizeof (mlt_mobile_register) / sizeof (mlt_mobile_register[0])); ++i)
    {
        mlt_mobile_register[i](repository);
    }
}
