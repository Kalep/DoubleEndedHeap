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
		Compare cmp;//�Ƚ�������
	public:
		DoubleEndedHeap(int MAX_SIZE = 1000):maxSize(MAX_SIZE),currentSize(0)
		{
			this->heap = new T[maxSize];
		};
		DoubleEndedHeap(const DoubleEndedHeap& other){
			//printf("����������heap\n");
			//system("pause");
			this->maxSize = other.maxSize;
			this->currentSize = other.currentSize;
			this->heap = new T[maxSize]; //����Ƚ�Σ��
			for(int i = 0; i<this->currentSize;i++)
				*(this->heap+ i) = other[i];
		}
		DoubleEndedHeap& operator=(const DoubleEndedHeap& other)
		{
			//printf("�����˵Ⱥ�heap\n");
			//system("pause");
			this->maxSize = other.maxSize;
			this->currentSize = other.currentSize;
			for(int i = 0;i< other.size() ;i++)
				*(this->heap+i) = other[i];
			return *this;
		}
		~DoubleEndedHeap(){
			//printf("\t����heap�ĵ�ַ:%d %d %d\n",this,this->heap,this->currentSize);
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
			//�������Լ��
			int currentIndex = this->currentSize -1;
			while(true)
			{
				//���ȿ��ǲ����Һ��ӽڵ㣬Ȼ����Լ������ֵܱȣ����Ƿ�Υ����һ��Լ��: ĳ�ڵ�����ӣ���ҪС�������Һ���
				if( (currentIndex&1) && cmp(data,*(heap+currentIndex-1))) //������Һ���(������), ���ֵܱ�����
				{
					//����
					 *(heap+currentIndex) = *(heap+currentIndex -1);
					 currentIndex--;
				}
				if( currentIndex <2 ) // currentIndex = 0 ,���� currentIndex = 1
					break;
				//�������岮���Ĺ�ϵ
				int grandpa = (((currentIndex>>1)-1)>>1) -1;//����4
				int uncleMin = (grandpa+1)<<1;//����2, 2(n+1)  ����Ϊ-1
				int uncleMax = uncleMin+1;//2n+3 �ǲ��ǲ�һ����
			
				if( cmp(data, *(heap+uncleMin) ) ) //�������廹С������
				{
					*(heap+currentIndex) = *(heap+uncleMin) ;
					currentIndex = uncleMin;
				}
				else if( cmp(*(heap+uncleMax) , data) ) //�������廹�󣬴���
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
			if(index&1){//�ҽڵ�
				this->deleteRight(index);
			}else{//��ڵ�
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
		void shiftLeftUp(int index){//�������֧���ϵ�
			//���游������
			int currentIndex = index;
			while(true){
				if(currentIndex&1) break;
				if( currentIndex ==0) break;//û�����ϵ���
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
		void shiftRightUp(int index){//�����ҷ�֧���ϵ�
			int currentIndex = index;
			while(true){
				if(!(currentIndex&1))
					break;
				int rightParentIndex = ((currentIndex-1)>>1) -1;
				if(rightParentIndex<1) return ;
				if(!(rightParentIndex&1))
					rightParentIndex++;
				if(cmp(*(heap+rightParentIndex),*(heap+currentIndex)))//Υ�� N Ӧ��С���游�Һ��ӵ�Լ��
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
		void shiftLeftDown(int index){//Դ����ߵĽڵ����
			 //1,�����һ���滻 ��ǰҪɾ����Ԫ��
			int currentIndex = index;
			//2,�ƶ�������Ŀ��
			T  shiftElement =  *(heap+currentIndex);
			while(true)
			{
				   //printf("size:%d\n",currentSize);
					//1,�ȸ��ұߵıȽϣ����Ƿ�Ҫ����
					if(!(currentIndex&1) && (currentIndex+1 < currentSize))
					{
						//printf("shiftLeftDown:%d\n",*(heap+currentIndex+1));
						if( cmp(*(heap+currentIndex+1),shiftElement))
						{
							T temp = *(heap+currentIndex+1);
							*(heap+currentIndex+1) = shiftElement;
							shiftElement = temp;
							this->shiftRightUp(currentIndex+1);//��������Һ���Ҫ����游�ڵ��Һ��ӵ�Լ���Ƿ�����
						}
					}
					//2,�Ҵ�С�Ľ������Լ������ӣ��Լ��ֵܵ�����
					int minChildIndex = (currentIndex+1)<<1; //Ĭ��ָ������
					if(minChildIndex >= currentSize) 
						break;
					int rightBrotherLeftChildIndex = ((currentIndex+1)+1)<<1;
					//��˭С
					if(rightBrotherLeftChildIndex < currentSize && cmp(*(heap+rightBrotherLeftChildIndex),*(heap+minChildIndex)))
						minChildIndex = rightBrotherLeftChildIndex;
					//���ǲ���Ҫ�ƶ�
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
			//��Ŀ��Ԫ�������һ��Ԫ���滻
			int currentIndex = index;
			//�ƶ����һ��Ԫ��
			T shiftElement = *(heap+currentIndex) ;
			while(true)
			{
				//1��������Ԫ�رȽ�
				if( (currentIndex&1) && cmp(shiftElement,*(heap+currentIndex-1)))
				{
					T temp = *(heap+currentIndex-1);
					*(heap+currentIndex-1) = shiftElement; //����һ�ߵ���������
					shiftElement = temp; //Ȼ��ı��ƶ���Ԫ��
					//printf("����������\n");
					this->shiftLeftUp(currentIndex-1);
				}
				int maxChildId = (((currentIndex-1) +1)<<1)+ 1; //Ĭ�������ֵܵ��Һ���
				if(maxChildId >= currentSize )
					break;
				int rightChildId = ((currentIndex+1)<<1) + 1;
				//2���Ƚ�������ѡ�Ĵδ�Ԫ�صĴ�С
				if(rightChildId < currentSize && cmp(*(heap+maxChildId),*(heap+rightChildId)))
					maxChildId = rightChildId;
				//3, �Ƚ����ĸ�Ŀ��ı�
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
			//�������һ��ռ��λ
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