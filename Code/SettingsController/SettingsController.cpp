#include "SettingsController.h"
#include <QSettings>
#include <QList>

QString SettingsController::savedSettingsPath="/home/lee/monkeyDoSettings.ini";
QString SettingsController::savedSelectionsPath="/home/lee/monkeyDoSelections.ini";

//Definition of variables declared private static in header
QList<TransformOperatorInfo> SettingsController::transformOperatorInfoList;
QList<TransformOperatorInfo> SettingsController::transformOperatorInfoList_pushed;
int SettingsController::defaultScopeIndex_pushed;
int SettingsController::defaultOperatorEngineIndex_pushed;
int SettingsController::defaultScopeIndex;
int SettingsController::defaultOperatorEngineIndex;

int SettingsController::updatedOperatorEngineIndex;
int SettingsController::updatedScopeIndex;

SettingsController::SettingsController(QObject *parent): QAbstractTableModel(parent)
{

}

void SettingsController::Initialise(QList<TransformOperatorInfo> transformOperatorInfoList)
{
    beginResetModel();  // =====

    SettingsController::transformOperatorInfoList=transformOperatorInfoList;
    SettingsController::loadSettings();

    endResetModel();    // =====
}

bool SettingsController::sortItemsByProviderName(bool reverseAlphabetical)
{
    beginResetModel();  // =====

    if (reverseAlphabetical)
    {
        std::sort(transformOperatorInfoList.begin(), transformOperatorInfoList.end(), TransformOperatorInfo::compareByDisplayNameDescending);
    }
    else
    {
        std::sort(transformOperatorInfoList.begin(), transformOperatorInfoList.end(), TransformOperatorInfo::compareByDisplayNameAscending);
    }

    endResetModel();    // =====

    return true;
}

QList<TransformOperatorInfo> SettingsController::getOrderedVisibleOperatorsList()
{
    QList<TransformOperatorInfo> list;
    foreach (auto TransformOperatorInfo, transformOperatorInfoList)
    {
        if (TransformOperatorInfo.isVisible)
        {
            list.append(TransformOperatorInfo);
        }
    }
    return list;
}

int SettingsController::getUpdatedOperatorIndex_userCombo()
{
    int index=getSettingsOperatorListIndexFromEngineIndex(updatedOperatorEngineIndex, true);
    if (index<0)
        index=0;

    return index;
}

int SettingsController::getOperatorIndexFromEngineIndex_userCombo(int engineIndex)
{
    int index=getSettingsOperatorListIndexFromEngineIndex(engineIndex, true);
    if (index<0)
        index=0;

    return index;
}

int SettingsController::getDefaultOperatorIndex_settingsCombo()
{
    int index=getSettingsOperatorListIndexFromEngineIndex(defaultOperatorEngineIndex, true);

    return index+1; // Indices in settings combobox are offset by 1 to make room for <use last> option
}

int SettingsController::getOperatorIndexFromEngineIndex_settingsCombo(int engineIndex)
{
    int index=getSettingsOperatorListIndexFromEngineIndex(engineIndex, true);

    return index+1;
}



int SettingsController::getSettingsOperatorListIndexFromEngineIndex(int engineIndex, bool onlyIncludeVisibleOperators)
{
    int settingsIndex=0;
    if (onlyIncludeVisibleOperators)
    {
        foreach (auto TransformOperatorInfo, transformOperatorInfoList)
        {
            if (TransformOperatorInfo.operatorEngineIndex==engineIndex)
            {
                if (TransformOperatorInfo.isVisible)
                    return settingsIndex;
                else
                    return -1;
            }
            if (TransformOperatorInfo.isVisible)
            {
                settingsIndex++;
            }
        }
    }
    else
    {
        foreach (auto TransformOperatorInfo, transformOperatorInfoList)
        {
            if (TransformOperatorInfo.operatorEngineIndex==engineIndex)
                return settingsIndex;

            settingsIndex++;
        }
    }
    return -1;
}

// defaultOperatorEngineIndex should be set to -1 for 'remember last'
// defaultScopeIndex should be set to -1 for 'remember last'
void SettingsController::saveSettings(int defaultOperatorEngineIndex, int defaultScopeIndex)
{   
    QSettings settings(savedSettingsPath, QSettings::IniFormat);
    settings.beginWriteArray("oplist order");
    for (int i=0; i<transformOperatorInfoList.size();i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("engineIndex", transformOperatorInfoList[i].operatorEngineIndex);
        settings.setValue("vis", transformOperatorInfoList[i].isVisible?"T":"F");
    }
    settings.endArray();

    settings.setValue("defaultOperator", defaultOperatorEngineIndex);
    settings.setValue("defaultScope", defaultScopeIndex);
    settings.sync();
}

int SettingsController::getUpdatedOperatorEngineIndex()
{
    return updatedOperatorEngineIndex;
}

int SettingsController::getUpdatedScopeIndex()
{
    return updatedScopeIndex;
}

void SettingsController::loadSettings()
{
    QList<TransformOperatorInfo> postLoadProviderInfoList;
    QSettings settings(savedSettingsPath, QSettings::IniFormat);
    int size = settings.beginReadArray("oplist order");

    // Reorder transform provider info list to reflect saved order, and also visibility
    for (int i = 0; i < size; i++)
    {
        settings.setArrayIndex(i);
        int transformEngineIndex = settings.value("engineIndex").toInt();
        bool visible = settings.value("vis").toString()=="T"?true:false;;

        int j=getOperatorSettingsListIndexFromEngineIndex(transformEngineIndex);
        if (j>=0)
        {
            transformOperatorInfoList[j].isVisible=visible;
            postLoadProviderInfoList.append(transformOperatorInfoList[j]);
            transformOperatorInfoList.removeAt(j);
        }
    }
    settings.endArray();

    // If the program offers any providers not specified in the settings file, add these to the list as visible
    // This could happen if a new version of the software is created with additional transform providers
    for (int j=0; j<transformOperatorInfoList.size();j++)
    {
        transformOperatorInfoList[j].isVisible=true;
        postLoadProviderInfoList.append(transformOperatorInfoList[j]);
    }
    transformOperatorInfoList=postLoadProviderInfoList;

    QSettings selectionSettings(savedSelectionsPath,QSettings::IniFormat);

    defaultOperatorEngineIndex=settings.value("defaultOperator","0").toInt();
    if (defaultOperatorEngineIndex>=0)
        updatedOperatorEngineIndex=defaultOperatorEngineIndex;
    else
        updatedOperatorEngineIndex=selectionSettings.value("operatorEngineIndex",0).toInt();

    defaultScopeIndex=settings.value("defaultScope","0").toInt();  // Default of 0 is "Name Only" option
    if (defaultScopeIndex>=0)
        updatedScopeIndex=defaultScopeIndex;
    else
        updatedScopeIndex=selectionSettings.value("scopeSelectionIndex",0).toInt();   // Default of 0 is "Name Only" option
}

int SettingsController::getDefaultOperatorEngineIndex()
{
    return defaultOperatorEngineIndex;
}

int SettingsController::getOperatorEngineIndexFromSettingsListIndex(int settingsListIndex)
{
    if ((settingsListIndex<0) or (settingsListIndex>=transformOperatorInfoList.size()))
        return -1;

    return transformOperatorInfoList[settingsListIndex].operatorEngineIndex;
}

int SettingsController::getOperatorSettingsListIndexFromEngineIndex(int engineIndex)
{
    int settingsListIndex = 0;
    foreach (auto setting, transformOperatorInfoList)
    {
        if (setting.operatorEngineIndex == engineIndex)
            return settingsListIndex;

        settingsListIndex++;
    }
    return -1;
}

void SettingsController::saveCurrentSelections(int operatorEngineIndex, int scopeSelectionIndex)
{
    QSettings settings(savedSelectionsPath,QSettings::IniFormat);
    // auto orderedVisibleOperatorList=getOrderedVisibleOperatorsList();
    // int operationID=orderedVisibleOperatorList[operatorEngineIndex].transformEngineIndex;

    settings.setValue("operatorEngineIndex", operatorEngineIndex);
    settings.setValue("scopeSelectionIndex", scopeSelectionIndex);
    settings.sync();
}

void SettingsController::pushSettingsState()
{
    transformOperatorInfoList_pushed=transformOperatorInfoList;
    defaultScopeIndex_pushed=defaultScopeIndex;
    defaultOperatorEngineIndex_pushed=defaultOperatorEngineIndex;
}

void SettingsController::popSettingsState()
{
    transformOperatorInfoList=transformOperatorInfoList_pushed;
    defaultScopeIndex=defaultScopeIndex_pushed;
    defaultOperatorEngineIndex=defaultOperatorEngineIndex_pushed;
}



///////////////////////////////////////////////////////
// Everything below here is view model functionality //
///////////////////////////////////////////////////////

int SettingsController::rowCount(const QModelIndex & /*parent*/) const
{
    return transformOperatorInfoList.length();
}

int SettingsController::columnCount(const QModelIndex & /*parent*/) const
{
    return 1;
}

QVariant SettingsController::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
        return transformOperatorInfoList[index.row()].displayName;

    if (role == Qt::CheckStateRole)
    {
        bool isEnabled = transformOperatorInfoList[index.row()].isVisible;
        return isEnabled? Qt::Checked : Qt::Unchecked;
    }

    return QVariant();
}

bool SettingsController::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() == 0 && role == Qt::CheckStateRole)
    {
        bool checked = value == Qt::Checked;
        transformOperatorInfoList[index.row()].isVisible=checked;
        emit this->dataChanged(index, index);
        return true;
    }
    return false; // TODO: Is this correct?
}

QVariant SettingsController::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal && section==0)
        return QString("Renaming options order / visibility");

    return QVariant();
}

Qt::ItemFlags SettingsController::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flag = QAbstractItemModel::flags(index);

    if (index.isValid())
        return (flag | Qt::ItemIsDragEnabled | Qt::ItemIsUserCheckable) & (~Qt::ItemIsDropEnabled);

    return flag | Qt::ItemIsDropEnabled;
}

Qt::DropActions SettingsController::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

bool SettingsController::moveRows(const QModelIndex& parent1, int source_first, int count, const QModelIndex& parent2, int dest)
{
    (void) parent1;
    (void) parent2;

    if (source_first==dest) return false;

    beginResetModel();  // =====

    QList<TransformOperatorInfo> removed;
    for (int c=0; c<count; c++)
    {
        auto sourceVal=this->transformOperatorInfoList[source_first]; // we don't add c since every individual remove shuffles things down
        this->transformOperatorInfoList.removeAt(source_first);
        removed.append(sourceVal);
    }

    if (dest>source_first)
    {
        dest-=count;
    }
    for (int c=0; c<count; c++)
    {
        this->transformOperatorInfoList.insert(dest+c,removed[c]);
    }

    endResetModel();    // =====

    auto modelIndex=index(0, 0);
    emit this->dataChanged(modelIndex, modelIndex);

    return true;
}

