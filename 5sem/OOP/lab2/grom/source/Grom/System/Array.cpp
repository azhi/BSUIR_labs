// Free open-source Grom library

#include "PrecompiledHeader.h"

#include "Utils.h"
#include "Array.h"

namespace Sys
{

// Auxiliary

inline
int SizeByCount(int count)
{
  return sizeof(void*) * count;
}

int STDCALL ComparePointers(void* p1, void* p2, void* userData)
{
  if (p1 > p2)
    return 1;
  else if (p1 < p2)
    return -1;
  else
    return 0;
}

// PointerArray

PointerArray::PointerArray() :
  m_Data(NULL), m_Capacity(0), m_Count(0)
{
}

PointerArray::~PointerArray()
{
  if (m_Data != NULL)
    free(m_Data);
  m_Data = NULL;
}

void PointerArray::SetCapacity(int capacity)
{
  if (m_Capacity != capacity)
  {
    if (capacity >= m_Count)
    {
      void* t = realloc(m_Data, SizeByCount(capacity));
      if (t == NULL && capacity > 0)
        throw new SystemException("out of memory");
      m_Data = (void**)t;
      m_Capacity = capacity;
    }
    else
      throw new ArgumentException("capacity cannot be less than count");
  }
}

void PointerArray::SetCount(int count)
{
  if (m_Count != count)
  {
    if (count >= 0)
    {
      if (count > m_Capacity)
        SetCapacity(count);
      if (count > m_Count)
        memset(m_Data + m_Count, 0, SizeByCount(count - m_Count));
      m_Count = count;
      if (count == 0)
        SetCapacity(0); // free m_Data
    }
    else
      throw CreateItemCountNegativeException(count);
  }
}

void* PointerArray::Item(int index) const
{
  ValidateIndex(index, m_Count);
  return m_Data[index];
}

void* PointerArray::operator[](int index) const
{
  return Item(index);
}

void PointerArray::SetItem(int index, void* item)
{
  ValidateIndex(index, m_Count);
  m_Data[index] = item;
}

void PointerArray::SetRange(int index, void** items, int count)
{
  ValidateRange(index, count, m_Count);
  if (count > 0)
  {
    if (items != NULL)
      memcpy(m_Data + index, items, SizeByCount(count));
    else
      throw CreateArgumentNullException();
  }
}

void PointerArray::Add(void* item)
{
  if (m_Capacity == m_Count)
    Grow();
  m_Data[m_Count] = item;
  ++m_Count;
}

void PointerArray::AddRange(void** items, int count)
{
  if (count > 0)
  {
    if (items != NULL)
    {
      if (m_Count + count > m_Capacity)
        SetCapacity(m_Count + count);
      memcpy(m_Data + m_Count, items, SizeByCount(count));
      m_Count += count;
    }
    else
      throw CreateArgumentNullException();
  }
  else if (count < 0)
    throw CreateItemCountNegativeException(count);
}

void PointerArray::Insert(int index, void* item)
{
  ValidateIndex(index, m_Count + 1);
  if (m_Capacity == m_Count)
    Grow();
  if (index < m_Count)
  {
    void** start = m_Data + index;
    memmove(start + 1, start, SizeByCount(m_Count - index));
  }
  m_Data[index] = item;
  ++m_Count;
}

void PointerArray::InsertRange(int index, void** items, int count)
{
  ValidateIndex(index, m_Count + 1);
  if (count > 0)
  {
    if (items != NULL)
    {
      if (m_Count + count > m_Capacity)
        SetCapacity(m_Count + count);   
      void** start = m_Data + index;
      if (index < m_Count)
        memmove(start + count, start, SizeByCount(m_Count - index));
      memcpy(start, items, SizeByCount(count));
      m_Count += count;
    }
    else
      throw CreateArgumentNullException();
  }
  else if (count < 0)
    throw CreateItemCountNegativeException(count);
}

bool PointerArray::Remove(void* item)
{
  int i = IndexOf(item);
  if (i >= 0)
    RemoveAt(i);
  return i >= 0;
}

void* PointerArray::RemoveAt(int index)
{
  void* result = Item(index); // also validates index
  void** start = m_Data + index;
  memmove(start, start + 1, SizeByCount(m_Count - 1 - index));
  --m_Count;
  return result;
}

void PointerArray::RemoveRange(int index, int count)
{
  ValidateIndex(index, m_Count);
  count = NormalizeRange(index, count, m_Count);
  if (count > 0)
  {
    if (index + count != m_Count)
    {
      void** start = m_Data + index;
      memmove(start, start + count, SizeByCount(m_Count - count - index));
    }
    m_Count -= count;
  }
  else if (count < 0)
    throw CreateItemCountNegativeException(count);
}

void PointerArray::Move(int sourceIndex, int targetIndex)
{
  NOT_IMPLEMENTED
}

void PointerArray::MoveRange(int sourceIndex, int count, int targetIndex)
{
  NOT_IMPLEMENTED
}

void PointerArray::CopyFrom(const PointerArray& sourceArray, int sourceIndex, int targetIndex, int count)
{
  ValidateRange(sourceIndex, count, sourceArray.m_Count);
  ValidateIndex(targetIndex, m_Count);
  SetCount(targetIndex);
  if (m_Count + count > m_Capacity)
    SetCapacity(m_Count + count);    
  memmove(m_Data + targetIndex, sourceArray.m_Data + sourceIndex, SizeByCount(count));
}

int PointerArray::ForwardSearch(void* item, int index, int count) const
{
  int result = -1;
  if (count > 0)
  {
    ValidateIndex(index, m_Count);
    ValidateRange(index, count, m_Count);
    for (int i = index; i < index + count; ++i)
    {
      if (m_Data[i] == item)
      {
        result = i;
        break;
      }
    }
  }
  else if (count < 0)
    throw CreateItemCountNegativeException(count);
  return result;
}

int PointerArray::BackwardSearch(void* item, int index, int count) const
{
  int result = -1;
  if (count > 0)
  {
    ValidateIndex(index, m_Count);
    ValidateRange(index, count, m_Count);
    for (int i = index + count - 1; i >= index; --i)
    {
      if (m_Data[i] == item)
      {
        result = i;
        break;
      }
    }
  }
  else if (count < 0)
    throw CreateItemCountNegativeException(count);
  return result;
}

int PointerArray::IndexOf(void* item, int startIndex, int count) const
{
  return ForwardSearch(item, startIndex, count);
}

int PointerArray::LastIndexOf(void* item, int startIndex, int count) const
{
  return BackwardSearch(item, startIndex, count);
}

int PointerArray::BinarySearch(void* item, int startIndex, int count, CompareProc* comparer, void* userData) const
{
  int result = ~startIndex;
  if (count > 0)
  {
    ValidateIndex(startIndex, m_Count);
    ValidateRange(startIndex, count, m_Count);
    int low = startIndex;
    int high = startIndex + count - 1;
    while (low <= high)
    {
      int mid = (low + high)/2;
      int comp = comparer(m_Data[mid], item, userData);
      if (comp < 0)
        low = mid + 1;
      else if (comp > 0)
        high = mid - 1;
      else
      {
        result = mid;
        break;
      }
    }
    if (result < 0)
      result = ~low;
  }
  else if (count < 0)
    throw CreateItemCountNegativeException(count);
  return result;
}

void PointerArray::Reverse(int startIndex, int count)
{
  ValidateIndex(startIndex, m_Count);
  count = NormalizeRange(startIndex, count, m_Count);
  if (count > 1)
  {
    void** start = m_Data + startIndex;
    void** end = start + count -1;
    for (; start < end; start++, end--)
    {
      void* val = *start;
      *start = *end;
      *end = val;
    }
  }
}

void PointerArray::Sort(int startIndex, int count, CompareProc* comparer, void* userData)
{
  ValidateIndex(startIndex, m_Count);
  count = NormalizeRange(startIndex, count, m_Count);
  DoSort(startIndex, startIndex + count - 1, comparer, userData);
}

void PointerArray::Grow()
{
  int delta;
  if (m_Capacity > 64)
    delta = m_Capacity / 4;
  else if (m_Capacity > 8)
    delta = 16;
  else
    delta = 4;
  SetCapacity(m_Capacity + delta);
}

void PointerArray::DoSort(int fromIndex, int toIndex, CompareProc* comparer, void* userData)
{
  int i, j, m;
  do
  {
    i = fromIndex;
    j = toIndex;
    m = (fromIndex + toIndex)/2;
    void* midItem = m_Data[m];
    do
    {
      while (comparer(m_Data[i], midItem, userData) < 0)
        ++i;
      while (comparer(midItem, m_Data[j], userData) < 0)
        --j;
      if (i <= j)
      {
        void* t = m_Data[i];
        m_Data[i] = m_Data[j];
        m_Data[j] = t;
        if (m == i)
          m = j;
        else if (m == j)
          m = i;
        ++i;
        --j;
      }
    }
    while (i <= j);
    if (fromIndex < j)
      DoSort(fromIndex, j, comparer, userData);
    fromIndex = i;
  }
  while (i < toIndex);
}

} // namespace Sys
