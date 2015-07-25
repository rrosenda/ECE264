#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "answer08.h"



SparseNode * SparseNode_create(int index, int value)
{
  if(value == 0)
  {
    return NULL;
  }
  SparseNode * array = malloc(sizeof(SparseNode));  
  array->index = index;
  array->value = value;
  array->left = NULL;
  array->right = NULL;
  return array;
}

SparseNode * SparseArray_insert(SparseNode * array, int index, int value)
{
  if(array == NULL)
    {
      return SparseNode_create(index, value);
    }else if(array->index == index)
    {
      array->value = value;
      return array;
    }else if(index > array->index)
    {
      array->right = SparseArray_insert(array->right, index, value);
    }else if(index < array->index)
    {
      array->left = SparseArray_insert(array->left, index, value);
    }
  return array;
}

SparseNode * SparseArray_build(int * indices, int * values, int length)
{
  SparseNode * array = NULL;
  
  int x = 0;
  while(x < length)
    {
      if(values[x] != 0)
      {
        array = SparseArray_insert(array, indices[x], values[x]);
      }
      x++;
    }
  return array;
}

SparseNode * SparseArray_remove_partition(SparseNode * array)
{ 
  if(array->left == NULL)
    {
      return array;
    }
  return SparseArray_remove_partition(array->left);
}

SparseNode * SparseArray_remove(SparseNode * array, int index)
{
  if(array == NULL)
    {
      return NULL;
    }
  if(array->index == index)
    {
      if(array->right == NULL && array-> left == NULL)
	    {
	       free(array);
	       return NULL;
	    }else if(array->left == NULL)
	    {    
	       SparseNode * node_NEW = array->right;
	       free(array);
	       return node_NEW;
	    }else if(array->right == NULL)
	    {
	       SparseNode * node_NEW = array->left;
	       free(array);
	       return node_NEW;
	    }
      SparseNode * node_NEW = SparseArray_remove_partition(array->right);
      array->index = node_NEW->index;
      array->value = node_NEW->value;
      array->right = SparseArray_remove(array->right, array->index);
      return array;
    }else if(index > array->index)
    {
      array->right = SparseArray_remove(array->right, index);
      return array;
    }
  array->left = SparseArray_remove(array->left, index);
  return array;
}

void SparseArray_destroy(SparseNode * array)
{
  if(array == NULL)
  {
    return;
  }
  SparseArray_destroy(array->left);
  SparseArray_destroy(array->right);
  free(array);
}


int SparseArray_getMin(SparseNode * array)
{
  if(array->left == NULL)
  {
      return array->index;
  }
  return SparseArray_getMin(array->left);
}

int SparseArray_getMax(SparseNode * array)
{
  if(array->right == NULL)
  {  
    return array->index;
  }
  return SparseArray_getMax(array->right);
}

SparseNode * SparseArray_getNode(SparseNode * array, int index)
{
  if(array == NULL)
  {
    return NULL;
  }else if(array->index == index)
  {
      return array;
  }else if(array->index > index)
  {
    return SparseArray_getNode(array->left, index);
  }
  return SparseArray_getNode(array->right, index);
}

SparseNode * SparseArray_copy(SparseNode * array)
{ 
  if(array == NULL)
  {
      return NULL;
  }
  SparseNode * temp = NULL;
  temp = malloc(sizeof(SparseNode));
  temp->index = array->index;
  temp->value = array->value;
  temp->right = SparseArray_copy(array->right);
  temp->left = SparseArray_copy(array->left);
  return temp;
}

SparseNode * SparseArray_merge_partition(SparseNode * array, SparseNode * temp)
{
  if(array == NULL)
  {
    return temp;
  }
  SparseNode * OPTION_MERGE = SparseArray_getNode(temp, array->index);
  if(OPTION_MERGE == NULL)
  {
    temp = SparseArray_insert(temp, array->index, array->value);
  }else
  {
     OPTION_MERGE->value = OPTION_MERGE->value + array->value;
     if(OPTION_MERGE->value == 0)
	   temp = SparseArray_remove(temp, OPTION_MERGE->index);
  }

  temp = SparseArray_merge_partition(array->left, temp);
  temp = SparseArray_merge_partition(array->right, temp);
  return temp;
}

SparseNode * SparseArray_merge(SparseNode * array_1, SparseNode * array_2)
{
  if(array_1 == NULL && array_2 == NULL)
  {
     return NULL;
  }else if(array_1 == NULL)
  {
    return SparseArray_copy(array_2);
  }else if(array_2 == NULL)
  {  
    return SparseArray_copy(array_1);
  }
  SparseNode * var_temp = SparseArray_copy(array_1);
  var_temp = SparseArray_merge_partition(array_2, var_temp);

  return var_temp;
}