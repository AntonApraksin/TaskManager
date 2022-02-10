FROM archlinux:latest

RUN	pacman -Syyu --noconfirm pkgconf make git cmake clang grpc boost gtest

RUN git clone https://github.com/AntonApraksin/TaskManager.git
 
WORKDIR "/TaskManager"

RUN git checkout fix/CmakeRestructuring

RUN cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -S. -Bbuild

RUN cmake --build ./build -- -j64

EXPOSE 50051

CMD ["./build/server", "--level", "trace"]
