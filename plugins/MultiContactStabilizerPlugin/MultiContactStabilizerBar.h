/**
   @author Kunio Kojima
*/
#pragma once

#include <cnoid/ToolBar>
#include <cnoid/Dialog>
#include <cnoid/SpinBox>
// #include <cnoid/Separator>
#include <cnoid/Buttons>
#include <cnoid/CheckBox>
#include <cnoid/LineEdit>
#include <QDialogButtonBox>

#include <cnoid/Archive>

#include "MultiContactStabilizerPlugin.h"

namespace cnoid {

class MultiContactStabilizerSetupDialog;
class MultiContactStabilizerPlugin;

class MultiContactStabilizerSetupDialog : public Dialog
{
public:
    QVBoxLayout* vbox;

    DoubleSpinBox errorCMWeightSpin;
    DoubleSpinBox errorMomentumWeightSpin;
    DoubleSpinBox errorAngularMomentumWeightSpin;
    DoubleSpinBox inputForceWeightSpin;
    DoubleSpinBox inputMomentWeightSpin;

    LineEdit errorCMNameLine;
    LineEdit errorMomentumNameLine;
    LineEdit errorAngularMomentumNameLine;
    LineEdit inputForceNameLine;
    LineEdit inputMomentNameLine;

    CheckBox saveParameterInFileNameCheck;

    MultiContactStabilizerSetupDialog();

    QHBoxLayout* newRow(QVBoxLayout* vbox);
    void storeState(Archive& archive);
    void restoreState(const Archive& archive);
};


class MultiContactStabilizerBar : public ToolBar
{
private:
    bool storeState(Archive& archive);
    bool restoreState(const Archive& archive);

public:
    MultiContactStabilizerSetupDialog* dialog;

    MultiContactStabilizerBar(MultiContactStabilizerPlugin* plugin);
};

}
