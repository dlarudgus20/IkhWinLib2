#include <IkhWinLib2/NonCopyable.h>
using namespace IkhProgram::IkhWinLib2;

class SomeClass : private NonCopyable
{
	// 이 클래스는 복사할 수 없습니다.
	// ...
};
