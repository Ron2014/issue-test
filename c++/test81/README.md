# discc for chinese test

## how to write makefile

https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

https://seisman.github.io/how-to-write-makefile/index.html

## distcc download

wget https://github.com/distcc/distcc/releases/download/v3.4/distcc-3.4.tar.gz

## install

yum install binutils-devel python3-devel -y

./autogen.sh
./configure && make && sudo make install

## run server

username=testuser
id $username >& /dev/null
if [ $? -ne 0 ]; then
    useradd $username
    passwd $username
    usermod -g wheel $username
fi
core=$(cat /proc/cpcinfo | grep processor | wc -l)
systemctl stop firewalld

export PATH=/usr/local/bin:$PATH
distccd -j$core --daemon --allow 192.168.0.0/8 --allow 127.0.0.1 --user $username --enable-tcp-insecure

## run client (build)

other_hosts=(192.168.132.188)
export DISTCC_POTENTIAL_HOSTS="localhost $(echo ${other_hosts[@]})"
make clean && pump make -j$core CXX='distcc g++'