#include "calcdialog.h"
#include "ui_calcdialog.h"

#include "main.h"
#include <QString>

calcDialog::calcDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::calcDialog)
{
    ui->setupUi(this);
	
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(pushButtonClicked()));
}

calcDialog::~calcDialog()
{
    delete ui;
}

// void calcDialog::setModel(ClientModel *model)
// {

// }

void calcDialog::pushButtonClicked()
{
    int nHowManyBlocks = 1000;

    QLocale us(QLocale::English, QLocale::UnitedStates);
    CBigNum bnAverageWeight = GetAverageWeightOverPeriod(nHowManyBlocks);
    qint64 nAverageWeight = bnAverageWeight.getint();

    //set network text even if user amount is not valid 
    // TODO: 1000 is a parameter
    QString strAvgSize = QString("The average weight staked over the last %1 blocks is %2.").arg(us.toString(nHowManyBlocks)).arg(bnAverageWeight.getuint());
    ui->avgWeightResult->setText(strAvgSize);

    if(ui->blockSizeEdit->text().isEmpty())
        return;

    int nTargetDays = ui->comboBoxTargetDays->currentText().toInt();
    QString strUserSize = ui->blockSizeEdit->text();
    int nUserAmount = strUserSize.toInt();

    //less than average weight
    if(nUserAmount * nTargetDays < nAverageWeight)
    {
        qint64 nUserWeightAtTarget = nUserAmount * nTargetDays;
        ui->avgWeightResult->setText(strAvgSize + QString(" It is recommended that you do not split your HOLD. %1 HOLD will yield a weight of <b>%2</b> on day %3, which is %4 less weight than the average weight staked in the last %5 blocks.")
                                     .arg(us.toString(nUserAmount))
                                     .arg(us.toString(nUserWeightAtTarget))
                                     .arg(us.toString(nTargetDays))
                                     .arg(us.toString(bnAverageWeight.getuint64() - nUserWeightAtTarget))
                                     .arg(us.toString(nHowManyBlocks))
                                    );
        return;
    }

    int nSplitCount = nUserAmount * nTargetDays / nAverageWeight;
    int nRecommendedSize = nUserAmount / nSplitCount;
    qint64 nUserWeightAtTarget = nRecommendedSize * nTargetDays;
    ui->avgWeightResult->setText(strAvgSize + QString(" It is recommended that you split your %1 HOLD into <b>%2 outputs of %3 HOLD</b>. This will yield a weight of <b>%4</b> on day %5.")
                            .arg(us.toString(nUserAmount))
                            .arg(us.toString(nSplitCount))
                            .arg(us.toString(nRecommendedSize))
                            .arg(us.toString(nUserWeightAtTarget))
                            .arg(us.toString(nTargetDays))
                            );
}

void calcDialog::on_buttonBox_accepted()
{
	close();
}
