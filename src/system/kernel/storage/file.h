//
//  file.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.03.2023.
//

#ifndef FILE_H
#define FILE_H

#include <gunwfile.h>

enum gnwFileErrorCode k_stor_file_getInfo(const data_t pathData, 
                                          struct gnwFileInfo * const fileInfo);

enum gnwFileErrorCode k_stor_file_load(const data_t pathData,
                                       ptr_t dst);

#endif // FILE_H
