#ifndef _TBSOURCE_H_
#define _TBSOURCE_H_

namespace tb
{

	class Source
	{

	public:

		Source();
		~Source();

		virtual bool AddBlock(float* a_Target) { return false; }

	}; // class Source

}; // namespace tb

#endif