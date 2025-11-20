#pragma once
#include <QDialog>
#include <QTabWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QDoubleSpinBox>

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent = nullptr);

private slots:
    void saveSettings();
    void loadSettings();

private:
    // Visualizer settings
    QLineEdit* m_presetPath = nullptr;
    QCheckBox* m_chkShuffle = nullptr;
    QCheckBox* m_chkHardCut = nullptr;
    QSpinBox* m_spinFPS = nullptr;
    QSpinBox* m_spinMeshX = nullptr;
    QSpinBox* m_spinMeshY = nullptr;
    QComboBox* m_spinTexSize = nullptr;
    QDoubleSpinBox* m_spinBeatSens = nullptr;
    QDoubleSpinBox* m_spinSmoothDur = nullptr;

    // Text overlay settings
    QCheckBox* m_chkShowArtist = nullptr;
    QCheckBox* m_chkShowWatermark = nullptr;
    QLineEdit* m_txtWatermark = nullptr;
    QDoubleSpinBox* m_spinGlobalScale = nullptr;

    // Recording settings
    QLineEdit* m_ffmpegTemplate = nullptr;
};