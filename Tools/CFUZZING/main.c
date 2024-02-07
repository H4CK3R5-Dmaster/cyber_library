#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bits/getopt_core.h>
#include <curl/curl.h>

#define MAX_KEYWORD_LENGTH 100
#define MAX_URL_LENGTH 256

void clear_progression()
{
    printf("\033[2K");
    printf("\033[A");
}

int test_url(const char *url, int total_urls, int current_url)
{
    CURL *curl;
    CURLcode res;
    long http_code = 0;

    curl = curl_easy_init();
    if (curl)
    {

        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        res = curl_easy_perform(curl);

        if (res == CURLE_OK)
        {
            
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            if (http_code == 200 || http_code == 302 || http_code == 301)
            {
                printf("URL fonctionnelle - Statut code : %ld - %s\n", http_code, url);
                
            }
            
        }

        curl_easy_cleanup(curl);
    }

    float percentage = (float)current_url / (float)total_urls * 100.0;

    printf("Progression : %.2f%%\n", percentage);
    clear_progression();

    return http_code == 200;
}

int count_keywords(const char *file_path)
{
    int count = 0;
    FILE *file = fopen(file_path, "r");
    if (file != NULL)
    {
        char keyword[MAX_KEYWORD_LENGTH];
        while (fgets(keyword, sizeof(keyword), file) != NULL)
        {
            count++;
        }
        fclose(file);
    }
    return count;
}

int main(int argc, char *argv[])
{
    printf(" ___________________ ___________________________.___ _______    ________ \n");
    printf("\\_   ___ \\_   _____/    |   \\____    /\\____    /|   |\\\\      /  _____/\n");
    printf("/    \\  \\/ |    __) |    |   / /     /   /     / |   |/   |   \\/   \\  ___ \n");
    printf("\\     \\____|     \\  |    |  / /     /_  /     /_ |   /    |    \\    \\_\\  \\\n");
    printf("\\______  /\\___  /  |______/ /_______ \\/_______ \\|___\\____|__  /\\______  /\n");
    printf("        \\/     \\/                    \\/        \\/            \\/        \\/ \n");
    printf("v 1.0\n");
    printf("by H4CK3R5-Dmaster \n\n");
    int opt;
    char base_url[MAX_URL_LENGTH];
    char wordlist_path[MAX_URL_LENGTH];

    while ((opt = getopt(argc, argv, "u:w:")) != -1)
    {
        switch (opt)
        {
        case 'u':
            strncpy(base_url, optarg, sizeof(base_url));
            printf("[+] -- TARGET URL : %s", base_url);
            printf("\n");

            break;
        case 'w':
            strncpy(wordlist_path, optarg, sizeof(wordlist_path));
            printf("[+] -- WordList : %s", wordlist_path);
            printf("\n\n");
            break;
        default:
            fprintf(stderr, "Usage: %s -u <base_url> -w <wordlist_path>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (strlen(base_url) == 0 || strlen(wordlist_path) == 0)
    {
        fprintf(stderr, "Les options -u et -w sont requises.\n");
        fprintf(stderr, "Usage: %s -u <base_url> -w <wordlist_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(wordlist_path, "r");
    if (file == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return EXIT_FAILURE;
    }
    char keyword[MAX_KEYWORD_LENGTH];
    int current_url = 0;
    int count = count_keywords(wordlist_path);
    while (fgets(keyword, sizeof(keyword), file) != NULL)
    {
        keyword[strcspn(keyword, "\n")] = '\0';

        char url[MAX_URL_LENGTH];
        snprintf(url, sizeof(url), "%s%s", base_url, keyword);
        int keylen = strlen(keyword);

        int result = test_url(url, count, ++current_url);

        if (result == 1)
        {
            printf("Résultat pour %s : %s\n", url, "Endpoint trouvé\n\n");
        }
    }
    fclose(file);

    return 0;
}