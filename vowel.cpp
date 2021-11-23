//This program counts all the vowels in specified text files
#include "settings.cpp"

std::map<char,int> vowels  = {
      {'a',0},
      {'e',0},
      {'i',0},
      {'o',0},
      {'u',0}
  };
pthread_mutex_t myMutex; //ensures only one file is open at a time

void* vowelCount(void* args) {
    std::ifstream inFile;
    std::string* p = (std::string*) args;
    std::string file =* p;
    delete p;
    pthread_mutex_lock(&myMutex);
    inFile.open(PATH+file);

    if (!inFile){
      std::cout << "Unable to open " << file << std::endl;
      exit(1);
    }
  
  char vowel;
  while (inFile >> std::noskipws >> vowel){
  	if (vowel == 'a' || vowel == 'A')
  	  vowels['a']+=1;
  	else if (vowel == 'e' || vowel == 'E')
  	  vowels['e']+=1;
  	else if (vowel == 'i' || vowel == 'I')
  	  vowels['i']+=1;
  	else if (vowel == 'o' || vowel == 'O')
  	  vowels['o']+=1;
  	else if (vowel == 'u' || vowel == 'U')
  	  vowels['u']+=1;
  }

  inFile.close(); 
  pthread_mutex_unlock(&myMutex);
  pthread_exit(NULL);
}

int main(){
  pthread_t thread[NUM_THREADS];
  std::stringstream fileName;
  std::string *str_p;
  pthread_mutex_init(&myMutex, 0);
  
  //creates a thread for each text file
  for (int n = 0; n < NUM_THREADS && n < NUM_FILES; n++){ 
    //Hard coded
    fileName <<"text"<<n+1<<".txt";
    str_p = new std::string(fileName.str());
    
  	if(pthread_create(&thread[n], NULL, vowelCount,(void*) str_p)){ 
  	  std::cout << "Error with thread creation" << std::endl;
  	  return -1;
  	}
  	fileName.str("");
  	fileName.clear();
  }
  
  for (int n = 0; n < NUM_THREADS && n < NUM_FILES; n++){
  	if(pthread_join(thread[n], NULL)){
  	  std::cout << "Error joining the "<<n<<" thread" << std::endl;
  	  return -1;
  	}
    
  }
  pthread_mutex_destroy(&myMutex);

  //Displaying the results
  for(auto& x:vowels) std::cout<<x.first<<":"<<x.second<<'\n';
  return 0;
}