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

size_t AppendDataToStringCurlCallback(void *ptr, size_t size, size_t nmemb, void *vstring);
string DownloadUrlAsString(const string & url);

void loadPlugin(string plugin);
void setIndentLength(int length);
void setIndentUseSpace(bool useSpace);

int main(int argc, const char * argv[])
{
    Value root;
    Reader reader;
    
    const string document = "{\"encoding\":\"UTF-8\",\"plug-ins\":[\"python\",\"c++\",\"ruby\"],\"indent\":{\"length\":3,\"use_space\":true}}";
    
    bool parsingSuccessful = reader.parse(document, root);
    
    if(!parsingSuccessful)
    {
        cout << "Failed to parse configuration\n" << reader.getFormatedErrorMessages();
        return 1;
    }
    
    string encoding = root.get("encoding","UTF-8").asString();
    
    const Value plugins = root["plug-ins"];
    
    int i = 0;
    
    for(i = 0; i < plugins.size(); ++i)
    {
        loadPlugin(plugins[i].asString());
    }
    
    setIndentLength( root["indent"].get("length", 3).asInt() );
    setIndentUseSpace( root["indent"].get("use_space", true).asBool() );
    
    root["encoding"] = "iso-8859-1";
    root["indent"]["length"] = 1;
    root["indent"]["use_space"] = false;
    
    StyledWriter writer;
    
    string outputConfig = writer.write(root);
    
    //cin >> root["subtree"];
    root["subtree"] = "subtree";
    cout << root << "\n";
    
    cout << DownloadUrlAsString("http://www.pogamadores.com");
    
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
    string * pstring = (string*)vstring;
    pstring->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

string DownloadUrlAsString(const string & url)
{
    string body;
    
    CURL *curl_handle;
    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, AppendDataToStringCurlCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &body);
    curl_easy_perform(curl_handle);
    curl_easy_cleanup(curl_handle);
    
    return body;
}