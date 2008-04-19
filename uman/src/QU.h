#ifndef QU_H_
#define QU_H_

#include <QObject>

//! This class provides global flags and enumerations.
class QU: QObject {
	Q_OBJECT
public:
	enum EventMessageType {
		information,
		warning,
		help,
		saving
	};
	Q_DECLARE_FLAGS(EventMessageTypes, EventMessageType)
	
	enum AudioTagTaskMode {
		useArtist,
		useTitle,
		useGenre,
		useYear
	};
	Q_DECLARE_FLAGS(AudioTagTaskModes, AudioTagTaskMode)
	
	enum RenameTaskMode {
		renameDirectory,
		renameDirectorySpecial, // use and preserve folder tags ([VIDEO], [SC], ...)
		renameSongFile,
		renameAudioFile,
		renameCoverFile,
		renameBackgroundFile,
		renameVideoFile,
		renameVideoFileSpecial // use videogap [VD#0]
	};
	Q_DECLARE_FLAGS(RenameTaskModes, RenameTaskMode)
	
	enum PreparatoryTaskMode {
		autoAssignFiles,
		removeUnsupportedTags
	};
	Q_DECLARE_FLAGS(PreparatoryTaskModes, PreparatoryTaskMode)
	
	enum ReportDataType {
		text,
		icon
	};
	Q_DECLARE_FLAGS(ReportDataTypes, ReportDataType)	
};

#endif /*QU_H_*/
