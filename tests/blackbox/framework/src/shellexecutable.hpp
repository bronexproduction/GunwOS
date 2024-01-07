//
//  shellexecutable.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 29.07.2022.
//

#ifndef SHELLEXECUTABLE_HPP
#define SHELLEXECUTABLE_HPP

#include <string>

class ShellExecutablePrivate;
class ShellExecutable {

    public:
        ShellExecutable();
        ~ShellExecutable();

    public:
        void Launch();

        int GetInFd();
        int GetOutFd();

    private:
        virtual const std::string BuildCommand() const = 0;
        
    private:
        ShellExecutablePrivate * const d;
};

#endif // SHELLEXECUTABLE_HPP
