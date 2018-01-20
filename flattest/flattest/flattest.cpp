// flattest.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <flatbuffers\flatbuffers.h>
#include "Test_generated.h"


int main()
{
	flatbuffers::FlatBufferBuilder fbb;
	MyMino::state s= MyMino::state::state_start;
	MyMino::Vec3 *pos=new MyMino::Vec3(1,1,1);
	MyMino::Mino a = MyMino::Mino::Mino_tmino;
	auto mi = MyMino::Createstart(fbb, pos, 150,100,fbb.CreateString("이재형"), false,a,s,0);

	auto tm = MyMino::Getstart(fbb.GetBufferPointer());
	printf("%d",tm->hp());
    return 0;
}

