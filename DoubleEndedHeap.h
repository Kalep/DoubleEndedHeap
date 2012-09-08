#ifndef _DOUBLE_ENDED_HEAP
#define _DOUBLE_ENDED_HEAP
#include <functional>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;


	template<typename T,typename Compare = less<T> >
	class DoubleEndedHeap
	{
	private:
		Compare cmp;//比较器对象
	public:
		DoubleEndedHeap(int MAX_SIZE = 1000):maxSize(MAX_SIZE),currentSize(0)
		{
			this->heap = new T[maxSize];
		};
		DoubleEndedHeap(const DoubleEndedHeap& other){
			//printf("拷贝构造了heap\n");
			//system("pause");
			this->maxSize = other.maxSize;
			this->currentSize = other.currentSize;
			this->heap = new T[maxSize]; //这个比较危险
			for(int i = 0; i<this->currentSize;i++)
				*(this->heap+ i) = other[i];
		}
		DoubleEndedHeap& operator=(const DoubleEndedHeap& other)
		{
			//printf("调用了等号heap\n");
			//system("pause");
			this->maxSize = other.maxSize;
			this->currentSize = other.currentSize;
			for(int i = 0;i< other.size() ;i++)
				*(this->heap+i) = other[i];
			return *this;
		}
		~DoubleEndedHeap(){
			//printf("\t析构heap的地址:%d %d %d\n",this,this->heap,this->currentSize);
			delete[] this->heap;
		}
		int size() const{
			return this->currentSize;
		}
		int max_size() const{
			return this->maxSize;
		}
		bool insert(T  data){
			this->currentSize++;
			if(this->currentSize > this->maxSize)
				return false;
			//检查三个约束
			int currentIndex = this->currentSize -1;
			while(true)
			{
				//首先看是不是右孩子节点，然后跟自己的左兄弟比，看是否违法第一个约束: 某节点的左孩子，都要小于它的右孩子
				if( (currentIndex&1) && cmp(data,*(heap+currentIndex-1))) //如果是右孩子(奇数项), 左兄弟比它大
				{
					//交换
					 *(heap+currentIndex) = *(heap+currentIndex -1);
					 currentIndex--;
				}
				if( currentIndex <2 ) // currentIndex = 0 ,或者 currentIndex = 1
					break;
				//检查跟叔叔伯伯的关系
				int grandpa = (((currentIndex>>1)-1)>>1) -1;//除以4
				int uncleMin = (grandpa+1)<<1;//乘以2, 2(n+1)  最顶层的为-1
				int uncleMax = uncleMin+1;//2n+3 是不是不一样呢
			
				if( cmp(data, *(heap+uncleMin) ) ) //比左叔叔还小，错误
				{
					*(heap+currentIndex) = *(heap+uncleMin) ;
					currentIndex = uncleMin;
				}
				else if( cmp(*(heap+uncleMax) , data) ) //比右叔叔还大，错误
				{
					*(heap+currentIndex) = *(heap+uncleMax) ;
					currentIndex = uncleMax;
				}
				else
					break;
			}
			*(heap+currentIndex) = data;
			return true;
		};
		bool empty() const{
			return currentSize == 0;
		}
		bool full() const{
			return currentSize == maxSize;
		}
		void copy(DoubleEndedHeap<T,Compare>& other)
		{
			this->currentSize = other.size();
			for( int index = 0 ; index <this->currentSize; index++)
				*(this->heap+index) = other[index];
		}
		void remove(int index)
		{
			if(index >= currentSize) 
				return ;
			if(index&1){//右节点
				this->deleteRight(index);
			}else{//左节点
				this->deleteLeft(index);
			}
		}
		void removeMin()
		{
			  if(currentSize<1)
					return ;
			 this->deleteLeft(0);
		};
		void removeMax()
		{
			if( currentSize<1) return;
			this->deleteRight(1);
		};
		void removeAll(){
			this->currentSize = 0;
		}
		T  Min(){
			if(currentSize >0)
				return *heap;
			else  throw "error";
		};
		T  Max(){
			if(currentSize >1)
				return * (heap+1);
			else if(currentSize > 0)
				return *heap;
			else  throw "error";
		};
		T& operator[](int index) const{
			return *(heap+index);
		}

		T* heapAddress() const{
			return heap;
		}
	private:
		void shiftLeftUp(int index){//沿着左分支往上调
			//找祖父的左孩子
			int currentIndex = index;
			while(true){
				if(currentIndex&1) break;
				if( currentIndex ==0) break;//没法往上调了
				int leftParentIndex = (currentIndex>>1)-1;
				if(leftParentIndex&1)
					leftParentIndex--;
				if(cmp(*(heap+currentIndex),*(heap+leftParentIndex)))
				{
					T temp = *(heap+currentIndex);
					*(heap+currentIndex) = *(heap+leftParentIndex);
					*(heap+leftParentIndex) = temp;
					currentIndex = leftParentIndex;
				}
				else 
					break;
			}
		}
		void shiftRightUp(int index){//沿着右分支往上调
			int currentIndex = index;
			while(true){
				if(!(currentIndex&1))
					break;
				int rightParentIndex = ((currentIndex-1)>>1) -1;
				if(rightParentIndex<1) return ;
				if(!(rightParentIndex&1))
					rightParentIndex++;
				if(cmp(*(heap+rightParentIndex),*(heap+currentIndex)))//违反 N 应该小于祖父右孩子的约束
				{
					T temp = *(heap+currentIndex);
					*(heap+currentIndex) = *(heap+rightParentIndex);
					*(heap+rightParentIndex) = temp;
					currentIndex = rightParentIndex;
				}
				else 
					break;
			}
		}
		void shiftLeftDown(int index){//源着左边的节点调整
			 //1,用最后一个替换 当前要删除的元素
			int currentIndex = index;
			//2,移动调整的目标
			T  shiftElement =  *(heap+currentIndex);
			while(true)
			{
				   //printf("size:%d\n",currentSize);
					//1,先跟右边的比较，看是否要交换
					if(!(currentIndex&1) && (currentIndex+1 < currentSize))
					{
						//printf("shiftLeftDown:%d\n",*(heap+currentIndex+1));
						if( cmp(*(heap+currentIndex+1),shiftElement))
						{
							T temp = *(heap+currentIndex+1);
							*(heap+currentIndex+1) = shiftElement;
							shiftElement = temp;
							this->shiftRightUp(currentIndex+1);//交换后的右孩子要检查祖父节点右孩子的约束是否满足
						}
					}
					//2,找次小的交换，自己的左孩子，自己兄弟的左孩子
					int minChildIndex = (currentIndex+1)<<1; //默认指向左孩子
					if(minChildIndex >= currentSize) 
						break;
					int rightBrotherLeftChildIndex = ((currentIndex+1)+1)<<1;
					//比谁小
					if(rightBrotherLeftChildIndex < currentSize && cmp(*(heap+rightBrotherLeftChildIndex),*(heap+minChildIndex)))
						minChildIndex = rightBrotherLeftChildIndex;
					//看是不是要移动
					if( cmp(*(heap+minChildIndex),shiftElement))
					{
						*(heap+currentIndex) = *(heap+minChildIndex);
						currentIndex = minChildIndex;
					}
					else
						break;
			}
			*(heap+currentIndex) = shiftElement;
		}
		void shiftRightDown(int index){
			//将目标元素用最后一个元素替换
			int currentIndex = index;
			//移动最后一个元素
			T shiftElement = *(heap+currentIndex) ;
			while(true)
			{
				//1，与左侧的元素比较
				if( (currentIndex&1) && cmp(shiftElement,*(heap+currentIndex-1)))
				{
					T temp = *(heap+currentIndex-1);
					*(heap+currentIndex-1) = shiftElement; //先让一边的子树满足
					shiftElement = temp; //然后改变移动的元素
					//printf("发生交换了\n");
					this->shiftLeftUp(currentIndex-1);
				}
				int maxChildId = (((currentIndex-1) +1)<<1)+ 1; //默认是左兄弟的右孩子
				if(maxChildId >= currentSize )
					break;
				int rightChildId = ((currentIndex+1)<<1) + 1;
				//2，比较两个候选的次大元素的大小
				if(rightChildId < currentSize && cmp(*(heap+maxChildId),*(heap+rightChildId)))
					maxChildId = rightChildId;
				//3, 比较最大的跟目标的比
				if(cmp(shiftElement,*(heap+maxChildId)))
				{
					*(heap+currentIndex) = *(heap+maxChildId);
					currentIndex = maxChildId;
				}
				else 
					break;
			}
			*(heap+currentIndex) = shiftElement;
		}
		void deleteLeft(int index){
			if(this->empty()) 
				return ;
			if(index == currentSize-1)
			{
				currentSize--;
				return ;
			}
			//先用最后一个占个位
			 *(heap+index)  = *(heap+(--currentSize));
			 this->shiftLeftUp(index);
			 this->shiftLeftDown(index);
		}
		void deleteRight(int index){
			if(this->empty()) 
				return ;
			if(index == currentSize-1)
			{
				currentSize--;
				return ;
			}
			 *(heap+index)  = *(heap+(--currentSize));
			 this->shiftRightUp(index);
			 this->shiftRightDown(index);
		}
	
		int maxSize;
		int currentSize;
		T* heap;
	};

#endif