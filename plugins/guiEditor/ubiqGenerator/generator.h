#pragma once

#include "../../../../qrrepo/repoApi.h"

#include "../../../qrutils/generator/abstractGenerator.h"

//have no idea why is this nessesary
namespace qReal {
class GraphicalModelAssistInterface;
}


namespace ubiq {
namespace generator {

class Generator : public utils::AbstractGenerator
{
public:
	/** Constructor.
	  @param templateDirPath Path to a directory with generation template.
	  @param outputDirPath Path to a directory.
	  @param logicalModel Logical model reference.
	  @param errorReporter Object to return errors to.
	  */
    Generator(QString const &outputDirPath
			, QString const &programName
			, qReal::LogicalModelAssistInterface const &logicalModel
            , qReal::GraphicalModelAssistInterface const &graphicalModel
            , qReal::ErrorReporterInterface &errorReporter\
			);

	virtual ~Generator();

	/// Starts generation.
	void generate();

private:

    void initGeneratingFiles();
    void saveGeneratedFiles();
    void generateMainSwitch();
    QString generateFrameRelatedSwitch(const qReal::Id &currentFrame);


    qReal::GraphicalModelAssistInterface const &mGraphicalModel;
    QString mResultTestFile;
};

}
}

