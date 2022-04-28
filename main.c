#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
int outofmemory = 0;
void trash(char **words)
{
    if(words)
    {
        int size = 0;
        for( ; ; size++)
        {
            if(*(words + size) == NULL) break;
        }
        for(int i = 0 ; i < size ; i++)
        {
            free(*(words + i));
        }
        free(words);
    }
}
int num_words(char **text1, char **text2)
{
    int size = 0;
    for( ; ; size++)
    {
        if(*(text1 + size) == NULL) break;
    }
    int size2 = 0;
    for( ; ; size2++)
    {
        if(*(text2 + size2) == NULL) break;
    }
    if(size > size2)    return 1;
    if(size < size2)    return 2;
    return 0;
}
void destroy(char ***words)
{
    if(words)
    {
        int size1 = 0;
        for( ; ; size1++)
        {
            if(*(words + size1) == NULL) break;
        }
        for(int k = 0 ; k < size1 ; k++)
        {
            trash(*(words + k));
        }
        free(words);
    }
}
int compare(char *text1, char *text2)
{
    int max;
    if(strlen(text1) > strlen(text2))   max = (int)strlen(text1);
    if(strlen(text1) <= strlen(text2))   max = (int)strlen(text2);
    for(int i = 0 ; i < max ; i++)
    {
        if(i == (int)strlen(text2)) return 2;
        if(i == (int)strlen(text1)) return 1;
        if(*(text1 + i) >= 'A' && *(text1 + i) <= 'Z' && *(text2 + i) >= 'a' && *(text2 + i) <= 'z')    return 1;
        if(*(text2 + i) >= 'A' && *(text2 + i) <= 'Z' && *(text1 + i) >= 'a' && *(text1 + i) <= 'z')    return 2;
        if(*(text2 + i) >= 'A' && *(text2 + i) <= 'Z' && *(text1 + i) >= 'A' && *(text1 + i) <= 'Z')
        {
            if(tolower(*(text1 + i)) < tolower(*(text2 + i)))   return 1;
            if(tolower(*(text1 + i)) > tolower(*(text2 + i)))   return 2;
        }
        if(*(text2 + i) >= 'a' && *(text2 + i) <= 'z' && *(text1 + i) >= 'a' && *(text1 + i) <= 'z')
        {
            if(*(text1 + i) < *(text2 + i)) return 1;
            if(*(text1 + i) > *(text2 + i)) return 2;
        }
    }
    return 0;
}
char **split_words(const char *text)
{
    if(!text)   return NULL;
    if(*text == '\0')   return NULL;
    int words = 0;
    int bylakropka = 0;
    for(int i = 0 ; i < (int)strlen(text) ; i++)
    {
        if(isalpha(*(text+i)))
        {
            int j;
            words++;
            for(j = i ; j < (int)strlen(text) ; j++)
            {
                if(!isalpha(*(text+j))) break;
            }
            i = j;
            if(*(text+j) == '.')
            {
                bylakropka = 1;
                break;
            }
        }
        if(*(text+i) == '.')
        {
            bylakropka = 1;
            break;
        }
    }
    if(words == 0 && bylakropka == 1)
    {
        char **words_ptr = calloc(1,sizeof(char*));
        if(words_ptr == NULL)
        {
            outofmemory = 1;
            return NULL;
        }
        *words_ptr = NULL;
        return words_ptr;
    } else if(bylakropka == 0)  return NULL;
    char **words_ptr = calloc(words + 1,sizeof(char*));
    if(!words_ptr)
    {
        outofmemory = 1;
        return NULL;
    }
    int count;
    int licznik = 0;
    for(int i = 0 ; i < (int)strlen(text) ; i++)
    {
        count = 0;
        if(*(text+i) == '.')    break;
        if(isalpha(*(text+i)))
        {
            for(int j = i ; j < (int)strlen(text) ; j++)
            {
                if(!isalpha(*(text+j)))break;
                count++;
            }
            *(words_ptr + licznik) = calloc(count + 1,1);
            if(!*(words_ptr + licznik))
            {
                for(int j = licznik -1 ; j>=0 ; j--)    free(*(words_ptr + j));
                free(words_ptr);
                outofmemory = 1;
                return NULL;
            }
            for(int j = 0 ; j <= count ; j++)
            {
                if(!isalpha(*(text+i)))
                {
                    *(*(words_ptr + licznik) + j) = '\0';
                    i--;
                    break;
                }
                *(*(words_ptr + licznik) + j) = *(text+i);
                i++;
            }
            licznik++;
        }
    }
    *(words_ptr + words) = NULL;
    return words_ptr;
}
int sort_words(char **words)
{
    if(!words)  return 1;
    int size = 0;
    for( ; ; size++)
    {
        if(*(words + size) == NULL) break;
    }
    if(size == 0)   return 1;
    for(int i = 0 ; i < size ; i++)
    {
        for(int j = 0 ; j < size - 1 ; j++)
        {
            if(compare(*(words + j),*(words + j + 1)) == 2)
            {
                char *temp = *(words + j);
                *(words + j) = *(words + j + 1);
                *(words + j + 1) = temp;
            }
        }
    }
    return 0;
}
int split_sentences(const char *text, char ****output)
{
    if (!text)
    {
        *output = NULL;
        return 1;
    }
    int sentences = 0;
    for (int i = 0; i < (int) strlen(text); i++)
    {
        if (*(text + i) == '.') sentences++;
    }
    if(sentences == 0)
    {
        *output = NULL;
        return 2;
    }
    *output = calloc(sentences + 1,sizeof(char*));
    if(!*output)
    {
        *output = NULL;
        return 3;
    }
    int zdanie = 0;
    int k = 0;
    for (int i = 0; i < (int) strlen(text); i++)
    {
        if(*(text + i) == '.' && zdanie < sentences)
        {
            *(*output + zdanie) = split_words(text + k);
            if(!*(*output + zdanie))
            {
                destroy(*output);
                *output = NULL;
                return 3;
            }
            zdanie++;
            k = i + 1;
        }
    }
    return 0;
}
int sort_sentences(char ***output)
{
    if(!output)  return 1;
    int size = 0;
    for( ; ; size++)
    {
        if(*(output + size) == NULL) break;
    }
    for(int i = 0 ; i < size ; i++) sort_words(*(output + i));
    if(size == 0)   return 1;
    if(size == 1)   return 0;
    for(int i = 0 ; i < size ; i++)
    {
        for(int j = 0 ; j < size - 1 ; j++)
        {
            if(num_words(*(output + j),*(output + j + 1)) == 1)
            {
                char **temp = *(output + j);
                *(output + j) = *(output + j + 1);
                *(output + j + 1) = temp;
            }
        }
    }
    return 0;
}

int main()
{
    char *input = malloc(1000);
    if(!input)
    {
        printf("Failed to allocate memory");
        return 8;
    }
    printf("Podaj zdanie/a zakonczone kropka");
    scanf("%999[^\n]",input);
    while(getchar()!='\n');
    *(input + strlen(input)) = '\0';
    char ***out;
    int res = split_sentences(input,&out);
    if(res == 3)
    {
        printf("Failed to allocate memory");
        free(input);
        return 8;
    }
    if(res == 2 || res == 1)
    {
        printf("Nothing to show");
        free(input);
        destroy(out);
        return 0;
    }
    int size = 0;
    for( ; ; size++)
    {
        if(*(out + size) == NULL) break;
    }
    if(sort_sentences(out) || size == 0)
    {
        printf("Nothing to show");
        free(input);
        destroy(out);
        return 0;
    }
    for(int i = 0 ; i < size; i++)
    {
        int size2 = 0;
        for( ; ; size2++)
        {
            if(*(*(out + i) + size2) == NULL) break;
        }
        if(size2 == 0)
        {
            printf("Nothing to show\n");
        }
        else
        {
            for(int j = 0 ; j < size2 ; j++)
            {
                printf("%s ",*(*(out + i) + j));
            }
            printf("\n");
        }
    }
    free(input);
    destroy(out);
    return 0;
}

