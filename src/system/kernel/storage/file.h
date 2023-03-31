//
//  file.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.03.2023.
//

#ifndef FILE_H
#define FILE_H

#include <gunwfile.h>

enum gnwFileErrorCode k_stor_file_getInfo(const char * const path, 
                                          const size_t pathLen, 
                                          struct gnwFileInfo * const fileInfo);

#endif // FILE_H
