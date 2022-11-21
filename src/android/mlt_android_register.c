//
// Created by apple on 2022/11/21.
//
#include "framework/mlt.h"
extern void mlt_register_core(mlt_repository repository);
extern void (* const func_ptr[])(mlt_repository repository);
void (* const func_ptr[])(mlt_repository repository) = {
        mlt_register_core
};
