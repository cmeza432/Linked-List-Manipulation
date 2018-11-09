#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "node.h"

int read_lines(char*** dest, int* num_lines, char *name[])
{
  char buffer[256];
  char** temp_arr;
  char* temp_line;
  
  *num_lines = 0;
  
  /* Try opening the file */
  FILE* fp = fopen(name[1], "r");
  if (fp == NULL)
  {
    printf("Error opening file for reading. Exiting.\n");
    return 0;
  }
 
  /* Read the file line by line into the buffer */
  while (fgets(buffer, sizeof(buffer), fp) != NULL)
  {
    int line_length = strlen(buffer);
    char* s;

    /* Check if it's a blank line and skip if it is */
    if (strcmp(buffer, "\r\n") == 0 || strcmp(buffer, "\n") == 0 || strcmp(buffer, "\n\r") == 0)
      continue;
    
    (*num_lines)++;
    
    /* Allocate or reallocate enough space for a pointer to each line we've come across thus far. */
    if (*dest == NULL)
    {
      temp_arr = malloc(*num_lines * sizeof(char*));
    }
    else
    {
      temp_arr = realloc(*dest, *num_lines * sizeof(char*));
    }
    
    if (!temp_arr)
    {
      printf("Error (re)allocating memory. Exiting.\n");
      return 0;
    }
    *dest = temp_arr;
    
    /* Allocate enough space for the current line. */
    temp_line = malloc(line_length * sizeof(char));
    if (!temp_line)
    {
      printf("Error allocating space for the line. Skipping the line.\n");
      (*num_lines)--;
      continue;
    }
    (*dest)[*num_lines-1] = temp_line;
    
    /* Copy the current line into the array */
    strcpy((*dest)[*num_lines-1], buffer);
    
    /* Get rid of that pesky newline for both Mac and Windows */
    s = (*dest)[*num_lines-1];
    if (s[line_length - 2] == '\r' || s[line_length - 2] == '\n')
      s[line_length - 2] = '\0';
    else if (s[line_length - 1] == '\n')
      s[line_length - 1] = '\0';
  }
  
  fclose(fp);
  
  return 1;
}


/* Free up lines of space  */
void free_lines(char*** a, int num)
{
	int i;
	for (i = 0; i < num; i++)
	{
		free((*a)[i]);
	}
	free(*a);
}

/* Gets first line of file and seperates them into double values */
int categorize(char ***final, int count, double list[])
{
	count = 0;
	double i;
	int s = strlen((*final)[0]);
	char *buffer;
	char *line = malloc(s*sizeof(char));
	strcpy(line, (*final)[0]);
	buffer = strtok(line, ",");
	while(buffer != NULL)
	{
		count++;
		i = atof(buffer);
		list[count-1] = i;
		buffer = strtok(NULL, ",");
	}
	free(line);
	return count;
}

/* Gets second line of file for nodes to insert */
int get_position(char***final, double number[50], int position[50])
{
	int size = 0;
	int s = strlen((*final)[1]);
	char N[50][256];
	int i = 0;
	int f = 0;
	char *buffer;
	char*sep;
	char *line = malloc(s*sizeof(char));
	strcpy(line, (*final)[1]);
	buffer = strtok(line, ",");
	/* Seperates values from each comma */
	while(buffer!= NULL)
	{
		int f = strlen(buffer);
		strcpy(N[i], buffer);
		buffer = strtok(NULL, ",");
		i++;
		size++;
	}
	/* Seperates each comma seperated value at colon */
	for(f; f < i; f++)
	{
		buffer = strtok(N[f], ":");
		while(buffer != NULL)
		{
			number[f] = atof(buffer);
			buffer = strtok(NULL, ":");
			
			position[f] = atof(buffer);
			buffer = strtok(NULL, ":");
		}
	}
	return size;
}

/* Gets third line of file to find position to delete from list */
int delete_node(char*** list, int delete[])
{
	int count = 0;
	char *buffer;
	int s = strlen((*list)[2]);
	char *line = malloc(s*sizeof(char));
	strcpy(line, (*list)[2]);
	buffer = strtok(line, ",");
	while(buffer != NULL)
	{
		delete[count] = atoi(buffer);
		count++;
		buffer = strtok(NULL, ",");
	}
	free(line);
	return count;
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Error. Not enough arguments.Exiting....");
		return -1;
	}
	
	Node *head = NULL;
	char **final = NULL;
	double list[50];
	int position[50];
	double add[50];
	int delete[50];
	int num_lines;
	int count;
	int i;
	int size = 0;
	int delete_size;
	
	read_lines(&final, &num_lines, argv);
	count = categorize(&final, count, list);
	/* Create node for each value from the file and link them */
	for(i = 0; i<count;i++)
	{
		head = insert_at_tail(head, list[i]);
	}
	printf("\n");
	print_simple_list(head);
	printf("\n");
	print_in_reverse(head);	
	printf("\n");
	size = get_position(&final, add,position);
	/* Inserts node at given position of file */
	for(i=0; i < size;i++)
		head = insert_at_position(head, add[i], position[i]);
	
	print_simple_list(head);	
	printf("\n");
	/* Deletes node at given position */
	delete_size = delete_node(&final, delete);
	for(i = 0; i < delete_size; i++)
		head = delete_at_position(head, delete[i]);
	
	print_simple_list(head);	
	printf("\n");
	/* Free up space */
	free_lines(&final, num_lines);
}