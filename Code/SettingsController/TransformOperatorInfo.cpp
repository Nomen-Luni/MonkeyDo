#include "TransformOperatorInfo.h"

TransformOperatorInfo::TransformOperatorInfo(int transformEngineIndex, QString displayName, bool isVisible)
{
    this->displayName=displayName;
    this->operatorEngineIndex=transformEngineIndex;
    this->isVisible=isVisible;
}

bool TransformOperatorInfo::compareByDisplayNameAscending(const TransformOperatorInfo& first, const TransformOperatorInfo& second)
{
    return first.displayName < second.displayName;
}

bool TransformOperatorInfo::compareByDisplayNameDescending(const TransformOperatorInfo& first, const TransformOperatorInfo& second)
{
    return first.displayName > second.displayName;
}
