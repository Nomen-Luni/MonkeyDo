#ifndef TransformOperatorINFO_H
#define TransformOperatorINFO_H

#include <QString>

class TransformOperatorInfo
{
public:
    TransformOperatorInfo(int transformEngineIndex, QString displayName, bool isVisible);
    static bool compareByDisplayNameAscending(const TransformOperatorInfo& first, const TransformOperatorInfo& second);
    static bool compareByDisplayNameDescending(const TransformOperatorInfo& first, const TransformOperatorInfo& second);

    QString displayName;
    int operatorEngineIndex;
    bool isVisible;

private:
    // TransformEngine() {}    //Static/singleton- not instanced
    //static const int maxTransformOperators = 50;
};

#endif // TransformOperatorINFO_H
