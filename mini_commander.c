#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>  
#include <string.h>
#include <locale.h>

void openFolder(char* pathFolder);
void deleteFolder(char* pathFolder);
void deleteFile(char* pathFile);

int main(void)
{
    //UTF-8
    setlocale(LC_ALL, "Russian");

    int choice = 0;
    char path[500];

    printf("to open the folder - 1. \n");
    printf("to delete the folder - 2. \n");
    printf("to delete the file - 3. \n");
    printf("command> ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            printf("Enter the path to the folder: ");
            scanf("%500s", path);
            openFolder(path);
            break;
        
        case 2:
            printf("Enter the path to the folder: ");
            scanf("%500s", path);
            deleteFolder(path);
            break;
        
        case 3:
           printf("Enter the path to the file: ");
           scanf("%500s", path);
           deleteFile(path);
           break;
        
        default:
            printf("Error: 404");
            break;
    }

}

//open Folder
void openFolder(char* pathFolder)
{
   //UTF-8
   setlocale(LC_ALL, "Russian");

   DIR *folder;
   struct dirent *entry;
   int files = 0;

   folder = opendir(pathFolder);
   if (folder == NULL)
   {
      perror("Unable to read directory");
      main();
   }

   while ( (entry = readdir(folder)))
   {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        files++;
        printf("File %3d: %s\n",
                files,
                entry->d_name
            );
   }

   closedir(folder);
   printf("\n");
   main();
}

//delete folder
void deleteFolder(char* pathFolder)
{
    //UTF-8
    setlocale(LC_ALL, "Russian");

    DIR* dir;
    struct dirent* entry;
    struct stat statbuf;
   
    //Open the directory
    if ((dir = opendir(pathFolder)) != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            //skip '.' and '..' directories
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            
            //create full path for current entry
            char fullPath[1024];
            snprintf(fullPath, sizeof(fullPath), "%s/%s", pathFolder, entry->d_name);

            //Get the status of the current entry
            stat(fullPath, &statbuf);

            //if it's a direct, call the func recursively
            if (S_ISDIR(statbuf.st_mode))
                deleteFolder(fullPath);
            else
              //if it's a file, delete it
               unlink(fullPath);

            if (fullPath != pathFolder)
                deleteFolder(pathFolder);
        }

        //close the directory and delete it
        closedir(dir);
        rmdir(pathFolder);
        printf("The folder has been successfully deleted.\n\n");
        main();
    } else {
        perror("Error opening directory.\n\n");
        main();
    }

}


//delete file
void deleteFile(char* pathFile)
{
    //UTF-8
    setlocale(LC_ALL, "Russian");

    if (remove(pathFile) == 0)
    {
        printf("File %s deleted successfully.\n", pathFile);
        main();
    }
    else 
    {
        printf("Error: Unable to delete the file.\n");
        main();
    }
}
