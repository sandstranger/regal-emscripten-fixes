//
//  othersyms.c
//  Regal
//
//  Created by Cass Everitt on 4/4/12.
//  Copyright (c) 2012 n/a. All rights reserved.
//

#include <stdio.h>

#define CALL( foo ) void foo () { fprintf( stderr, #foo "\n" ); }

//CALL( gll_noop )
//CALL( CGLOpenCLMuxLockDown )
//CALL( glFlushRenderAPPLE )
//CALL( glFinishRenderAPPLE )
//CALL( CGLTexImageIOSurface2D )
//CALL( CGLOpenCLMuxLockDown )
//CALL( CGLGetSurface )
//CALL( CGLSetSurface )
//CALL( CGL )
CALL( gluOrtho2DCTX )


