#include "HTMLParser.h"

#include <string>

using std::string;

bool HTMLParser::isWhiteSpace(char ch){
    return ch == ' '  ||
           ch == '\t' ||
           ch == '\n' ||
           ch == '\r' ||
           ch == '\f';
}

char HTMLParser::toLowerCase(char ch){
    if(ch >= 'A' && ch <= 'Z'){
        return ch + ('a' - 'A');
    }
    return ch;
}

bool HTMLParser::startsWith(const string& html,int index,const string& word){
    if(index + word.length() > html.length()){
        return false;
    }
    for(int i = 0; i < word.length(); i++){
        if(toLowerCase(html[index + i]) != toLowerCase(word[i])){
            return false;
        }
    }
    return true;
}

bool HTMLParser::isTagBoundary(const string& html,int index){
    if(index >= html.length()){
        return true;
    }
    return isWhiteSpace(html[index]) || html[index] == '>';
}

int HTMLParser::findBodyStart(const string& html){
    for(int i = 0; i < html.length(); i++)
    {
        if(startsWith(html, i, "<body"))
        {
            int end = i + 5;

            if(!isTagBoundary(html, end)){
                continue;
            }
            while(end < html.length() && html[end] != '>'){
                end++;
            }
            if(end < html.length()){
                return end + 1;
            }
            return -1;
        }
    }
    return -1;
}

string HTMLParser::trim(const string& text){
    int start = 0;
    while(start < text.length() && isWhiteSpace(text[start])){
        start++;
    }

    if(start == text.length()){
        return "";
    }

    int end = text.length() - 1;
    while(end >= start && isWhiteSpace(text[end])){
        end--;
    }
    string result;
    for(int i = start; i <= end; i++){
        result += text[i];
    }
    return result;
}

DynamicArray<string> HTMLParser::extractText(const string& html){
    DynamicArray<string> text;
    int index = findBodyStart(html);
    if(index == -1){
        return text;
    }
    while(index < html.length())
    {
        //--------------------------------------------------
        // Stop when </body> is reached
        //--------------------------------------------------
        if(startsWith(html, index, "</body") && isTagBoundary(html, index + 6)){
            break;
        }
        //--------------------------------------------------
        // Handle HTML tags
        //--------------------------------------------------
        if(html[index] == '<')
        {
            //--------------------------------------------------
            // Skip HTML comments
            //--------------------------------------------------
            if(startsWith(html, index, "<!--")){
                index += 4;
                while(index + 2 < html.length()){
                    if(html[index] == '-' && html[index + 1] == '-' && html[index + 2] == '>'){
                        index += 3;
                        break;
                    }
                    index++;
                }
                continue;
            }
            //--------------------------------------------------
            // Skip <script> ... </script>
            //--------------------------------------------------
            if(startsWith(html, index, "<script") && isTagBoundary(html, index + 7)){
                while(index < html.length() && html[index] != '>'){
                    index++;
                }
                if(index < html.length()){
                    index++;
                }
                while(index < html.length())
                {
                    if(startsWith(html, index, "</script") && isTagBoundary(html, index + 8)){
                        while(index < html.length() && html[index] != '>'){
                            index++;
                        }
                        if(index < html.length()){
                            index++;
                        }
                        break;
                    }
                    index++;
                }
                continue;
            }
            //--------------------------------------------------
            // Skip <style> ... </style>
            //--------------------------------------------------
            if(startsWith(html, index, "<style") && isTagBoundary(html, index + 6))
            {
                while(index < html.length() && html[index] != '>'){
                    index++;
                }

                if(index < html.length()){
                    index++;
                }

                while(index < html.length())
                {
                    if(startsWith(html, index, "</style") && isTagBoundary(html, index + 7))
                    {
                        while(index < html.length() && html[index] != '>'){
                            index++;
                        }

                        if(index < html.length()){
                            index++;
                        }
                        break;
                    }
                    index++;
                }

                continue;
            }

            //--------------------------------------------------
            // Skip normal HTML tag
            //--------------------------------------------------
            while(index < html.length() && html[index] != '>'){
                index++;
            }

            if(index < html.length()){
                index++;
            }
            continue;
        }
        //--------------------------------------------------
        // Read visible text until next tag
        //--------------------------------------------------
        string currentText;
        bool hasText = false;

        while(index < html.length() && html[index] != '<')
        {
            currentText += html[index];
            if(!isWhiteSpace(html[index])){
                hasText = true;
            }
            index++;
        }
        if(hasText){
            currentText = trim(currentText);
            if(!currentText.empty()){
                text.push_back(currentText);
            }
        }
    }
    return text;
}