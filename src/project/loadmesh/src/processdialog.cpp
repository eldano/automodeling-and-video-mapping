#include "processdialog.h"

#include "ProcessMesh.h"

ProcessDialog::ProcessDialog(QWidget *parent)
    : QWidget(parent)
{
    // POISSON DISK SAMPLING GROUP
    QGroupBox *fileGroup = new QGroupBox(tr("Source Object file"));
    QLabel *filePathLabel = new QLabel(tr("Path:"));
    filePathEdit = new QLineEdit;
    searchButton = new QPushButton(tr("Browse"));

    /*QGridLayout *fileLayout = new QGridLayout;
    fileLayout->addWidget(keyLabel, 0, 0);
    fileLayout->addWidget(sourceFileEdit, 0, 1);
    fileLayout->addWidget(searchButton, 0, 2);
    fileGroup->setLayout(fileLayout);*/

    QHBoxLayout *fileLayout = new QHBoxLayout;
    fileLayout->addWidget(filePathLabel);
    fileLayout->addWidget(filePathEdit);
    fileLayout->addWidget(searchButton);
    fileGroup->setLayout(fileLayout);

    // POISSON DISK SAMPLING GROUP
    QGroupBox *poissonDiskGroup = new QGroupBox(tr("Poisson Disk-Sampling"));
    QLabel *sampleNumLabel = new QLabel(tr("Sample Num:"));
    sampleNumSpinBox = new QSpinBox;

    QLabel *radiusLabel = new QLabel(tr("Radius:"));
    radiusSpinBox = new QDoubleSpinBox;

    subsampleCheckBox = new QCheckBox(tr("SubSample?"));

    QHBoxLayout *samplesLayout = new QHBoxLayout;
    samplesLayout->addWidget(sampleNumLabel);
    samplesLayout->addWidget(sampleNumSpinBox);

    QHBoxLayout *radiusLayout = new QHBoxLayout;
    radiusLayout->addWidget(radiusLabel);
    radiusLayout->addWidget(radiusSpinBox);

    QVBoxLayout *poissonDiskLayout = new QVBoxLayout;
    poissonDiskLayout->addLayout(samplesLayout);
    poissonDiskLayout->addWidget(subsampleCheckBox);
    poissonDiskLayout->addLayout(radiusLayout);
    poissonDiskGroup->setLayout(poissonDiskLayout);

    // NORMAL COMPUTE GROUP
    QGroupBox *normalGroup = new QGroupBox(tr("Normal Extrapolation"));
    QLabel *neigborsLabel = new QLabel(tr("Neigbors (K):"));
    neigborsSpinBox = new QSpinBox;

    QHBoxLayout *neigborsLayout = new QHBoxLayout;
    neigborsLayout->addWidget(neigborsLabel);
    neigborsLayout->addWidget(neigborsSpinBox);

    flipFlagCheckBox = new QCheckBox(tr("Flip flag?"));

    QVBoxLayout *normalExtrapLayout = new QVBoxLayout;
    normalExtrapLayout->addLayout(neigborsLayout);
    normalExtrapLayout->addWidget(flipFlagCheckBox);
    normalGroup->setLayout(normalExtrapLayout);

    // POISSON RECONSTRUCTION GROUP
    QGroupBox *poissonReconstrGroup = new QGroupBox(tr("Poisson Reconstruction"));

    QLabel *octdepthLabel = new QLabel(tr("Otcet Depth:"));
    octdepthSpinBox = new QSpinBox;
    QHBoxLayout *octetLayout = new QHBoxLayout;
    octetLayout->addWidget(octdepthLabel);
    octetLayout->addWidget(octdepthSpinBox);

    QLabel *solverdivideLabel = new QLabel(tr("Solver divide:"));
    solverdivideSpinBox = new QSpinBox;
    QHBoxLayout *solverdivideLayout = new QHBoxLayout;
    solverdivideLayout->addWidget(solverdivideLabel);
    solverdivideLayout->addWidget(solverdivideSpinBox);

    QLabel *samplespernodeLabel = new QLabel(tr("Samples per node:"));
    samplespernodeSpinBox = new QDoubleSpinBox;
    QHBoxLayout *samplespernodeLayout = new QHBoxLayout;
    samplespernodeLayout->addWidget(samplespernodeLabel);
    samplespernodeLayout->addWidget(samplespernodeSpinBox);

    QLabel *offsetLabel = new QLabel(tr("Offset:"));
    offsetSpinBox = new QDoubleSpinBox;
    QHBoxLayout *offsetLayout = new QHBoxLayout;
    offsetLayout->addWidget(offsetLabel);
    offsetLayout->addWidget(offsetSpinBox);


    QVBoxLayout *poissonRecontrLayout = new QVBoxLayout;
    poissonRecontrLayout->addLayout(octetLayout);
    poissonRecontrLayout->addLayout(solverdivideLayout);
    poissonRecontrLayout->addLayout(samplespernodeLayout);
    poissonRecontrLayout->addLayout(offsetLayout);
    poissonReconstrGroup->setLayout(poissonRecontrLayout);

    buttonBox = new QDialogButtonBox(   QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(fileGroup);
    mainLayout->addWidget(poissonDiskGroup);
    mainLayout->addWidget(normalGroup);
    mainLayout->addWidget(poissonReconstrGroup);
    mainLayout->addStretch(1);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Process Mesh"));

    connect(searchButton, SIGNAL(clicked()), this, SLOT(setOpenFileName()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptPressed()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(rejectPressed()));

    init();
}

void ProcessDialog::init(){
    sampleNumSpinBox->setValue(1000);
    radiusSpinBox->setValue(0);
    subsampleCheckBox->setChecked(true);

    neigborsSpinBox->setValue(10);
    flipFlagCheckBox->setChecked(false);

    octdepthSpinBox->setValue(6);
    solverdivideSpinBox->setValue(6);
    samplespernodeSpinBox->setValue(1);
    offsetSpinBox->setValue(1);
}

void ProcessDialog::setOpenFileName()
{
    QFileDialog::Options options;
    //if (!native->isChecked())
    //    options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Open Object file"),
                                filePathEdit->text(),
                                tr("OBJ Files (*.obj)"),
                                &selectedFilter,
                                options);
    if (!fileName.isEmpty())
        filePathEdit->setText(fileName);
}

void ProcessDialog::acceptPressed(){

    string fileName = filePathEdit->text().toStdString();

    CMeshO cm;
    cm.Tr.SetIdentity();
    cm.sfn=0;
    cm.svn=0;
    cm.currentDataMask = MM_NONE;
    cm.currentDataMask |= MM_VERTCOORD | MM_VERTNORMAL | MM_VERTFLAG ;
    cm.currentDataMask |= MM_FACEVERT  | MM_FACENORMAL | MM_FACEFLAG ;

    //int mask = 0;
    bool modelOpened = ProcessMesh::open(fileName, (&cm));
    printf("MODEL OPENED==== %d\n", modelOpened);
    cout << "number of vertices " <<  cm.vert.size() << endl;
    cout << "number of faces " << cm.face.size()<< endl<< endl;

    printf("=== ProcessMesh::PoissonDiskSampling\n");
    bool subsampleFlag = subsampleCheckBox->isChecked();
    int sampleNum = sampleNumSpinBox->value();
    float radius = radiusSpinBox->value();
    CMeshO *newCmesh = ProcessMesh::PoissonDiskSampling((&cm), sampleNum, subsampleFlag, radius, 20, false);
    if (newCmesh != NULL){
        cout << "SALIDA POISSON::: " << endl;
        cout << "number of vertices " <<  newCmesh->vert.size() << endl;
        cout << "number of faces " << newCmesh->face.size()<< endl<< endl;

        bool modelSaved = ProcessMesh::save(string("CMESH"), newCmesh);
        printf("MODEL SAVED?? ==== %d\n\n", modelSaved);
    } else
        printf("=== NEW MESH NULL\n\n");

    printf("=== ProcessMesh::ComputeNormalsForPointSet\n");
    newCmesh = ProcessMesh::ComputeNormalsForPointSet(newCmesh, 10, false);
    if (newCmesh != NULL){
        cout << "SALIDA NORMALS::: " <<  endl;
        cout << "number of vertices " <<  newCmesh->vert.size() << endl;
        cout << "number of faces " << newCmesh->face.size()<< endl<< endl;

        bool modelSaved = ProcessMesh::save(string("CMESH"), newCmesh);
        printf("MODEL NORMALIZED SAVED?? ==== %d\n", modelSaved);
    } else
        printf("=== NEW MESH NULL\n");

    printf("=== ProcessMesh::SurfaceReconstructionPoisson\n");
	int OctDepth=octdepthSpinBox->value();
	float SamplesPerNode = samplespernodeSpinBox->value();
	int SolverDivide = solverdivideSpinBox->value();
	float Offset = offsetSpinBox->value();
    CMeshO *finalCmesh = ProcessMesh::SurfaceReconstructionPoisson(newCmesh, OctDepth, SolverDivide, SamplesPerNode, Offset);
    if (finalCmesh != NULL){
        cout << "SALIDA SURFACE::: " <<  endl;
        cout << "number of vertices " <<  finalCmesh->vert.size() << endl;
        cout << "number of faces " << finalCmesh->face.size()<< endl<< endl;

        bool modelSaved = ProcessMesh::save(string("FINALMESH"), finalCmesh);
        printf("FINAL MODEL SAVED?? ==== %d\n", modelSaved);
    } else
        printf("=== finalCmesh MESH NULL\n");

    //hide();
}

void ProcessDialog::rejectPressed(){
    //hide();
}
