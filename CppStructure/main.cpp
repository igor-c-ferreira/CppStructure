//
//  main.cpp
//  CppStructure
//
//  Created by Igor Ferreira on 1/10/14.
//  Copyright (c) 2014 Igor Ferreira. All rights reserved.
//

#include <iostream>
#include <curl/curl.h>
#include "json.h"

using namespace std;
using namespace Json;

/**
Method called during the download of the content.
Basically, append the new byte (char) to the already downloaded content

@param  ptr     The last download buffer
@param  size    Size of the minnor buffer unit
@param  nmemb   Number of units in the buffer
@param  vstring Already downloaded buffer
@return Size of handled information
*/
size_t AppendDataToStringCurlCallback(void *ptr, size_t size, size_t nmemb, void *vstring);

/**
Method used to config curl to download the content of an url as a string

@param  url The path of the content
@return The content of that url as string
*/
string DownloadUrlAsString(const string & url);

/**
Just print the content of the plugin content
@param  plugin  Content
*/
void loadPlugin(string plugin);
/**
Just print the content of the length content
@param  length  Content
*/
void setIndentLength(int length);
/**
Just print the content of the useSpace content
@param  useSpace  Content
*/
void setIndentUseSpace(bool useSpace);

int main(int argc, const char * argv[])
{
    Value root;
    Reader reader;
    
    //Download the json example
    const string document = DownloadUrlAsString("https://dl.dropboxusercontent.com/u/16397611/example.json");
    
    cout << document;
    
    //Parse the json
    bool parsingSuccessful = reader.parse(document, root);
    
    if(!parsingSuccessful)
    {
        cout << "Failed to parse configuration\n" << reader.getFormatedErrorMessages();
        return 1;
    }
    
    //Get the content for the encoding key, using UTF-8 as the default result
    string encoding = root.get("encoding","UTF-8").asString();
    
    //Get the value (array) for the plug-ins key
    const Value plugins = root["plug-ins"];
    
    int i = 0;
    
    for(i = 0; i < plugins.size(); ++i)
    {
        loadPlugin(plugins[i].asString());
    }
    
    setIndentLength( root["indent"].get("length", 3).asInt() );
    setIndentUseSpace( root["indent"].get("use_space", true).asBool() );
    
    //Change the values of the json object
    root["encoding"] = "iso-8859-1";
    root["indent"]["length"] = 1;
    root["indent"]["use_space"] = false;
    
    //Create a write to output a formatted json string
    StyledWriter writer;
    string outputConfig = writer.write(root);
    cout << outputConfig << "\n";
    
    //Create a new key in the json
    root["subtree"] = "subtree";
    cout << root << "\n";
    
    return 0;
}

void loadPlugin(string plugin)
{
    cout << plugin << "\n";
}

void setIndentLength(int length)
{
    cout << length << "\n";
}
void setIndentUseSpace(bool useSpace)
{
    cout << useSpace << "\n";
}

size_t AppendDataToStringCurlCallback(void *ptr, size_t size, size_t nmemb, void *vstring)
{
    string * pstring = (string*)vstring;    //Convert the void pointer to a string pointer to be able to use append
    pstring->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

string DownloadUrlAsString(const string & url)
{
    string body;
    
    CURL *curl_handle;
    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str()); //Set the request URL
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, AppendDataToStringCurlCallback); //Tell to the handler with function will be used to handle the buffer content
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &body); //Tell to the handler with memory space will be used as buffer
    curl_easy_perform(curl_handle); //Perform the block
    curl_easy_cleanup(curl_handle); //Clear the memory
    
    return body;
}