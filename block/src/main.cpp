#include "true_block.hpp"
#include "true_blockchain.hpp"
#include <cstring>
#include <cstdlib>

#include <time.h> //clock함수가 있는 헤더

//#include <emscripten.h>

using namespace std;

#ifdef __EMSCRIPTEN__   //엠스크립튼으로 컴파일할 경우에만
#include <emscripten.h> //emscripten.h 헤더파일을 추가
#endif

#ifdef __cplusplus //해당 모듈이 c++ 모듈일 경우 아래 매크로 선언
extern "C"         //네임 맹글러 X
{
#endif

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE
#endif
    void start_block()
    {

        BlockChain bc = BlockChain(0); //제네시스 블록 생성
        for (int i = 1; i <= 100; i++)
        {
            vector<string> v; //한 블록에 대한 넣고 싶은 데이터
            v.push_back("a");

            int index = bc.getNumOfBlocks();

            auto pair = findHash(index, bc.getLatestBlockHash(), v);

            bc.addBlock(bc.getNumOfBlocks(), bc.getLatestBlockHash(), pair.first, pair.second, v);
        }
  
    }

    int main()
    {
       //for(int i = 0; i<=10; i++){
        
            clock_t start, end;

            start = clock();

            start_block();

            end = clock();

            double time_taken = (double)(end - start) / (double)(CLOCKS_PER_SEC);

            printf("%f\n", time_taken);
       // }
    }

#ifdef __cplusplus
}
#endif