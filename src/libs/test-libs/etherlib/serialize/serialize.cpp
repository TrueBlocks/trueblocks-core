#include "utillib.h"
#include "options.h"
#include "person.h"

CPerson leader("Leader", 100);
CPerson *lastAdded = &leader;

int main(int argc, const char *argv[])
{

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    //while (!options.commandList.empty())
    {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        for (uint32_t i = 0 ; i < 10 ; i++) {
            CPerson *newPerson = new CPerson("Person " + asStringU(i), i * 2);
            lastAdded->next = newPerson;
            lastAdded = newPerson;
        }

        cout << "Creation\n" << SFString('-', 80) << "\n";
        CPerson *cur = &leader;
        while (cur) {
            cout << cur->name << " : " << cur->age << "\n";
            cur = cur->next;
        }
        cout.flush();

        SFArchive out(WRITING_ARCHIVE);
        if (out.Lock("./file.bin", binaryWriteCreate, LOCK_WAIT)) {
            leader.Serialize(out);
            out.Release();
        }

        cout << "\nReset\n" << SFString('-', 80) << "\n";
        leader = CPerson();
        cur = &leader;
        while (cur) {
            cout << cur->name << " : " << cur->age << "\n";
            cur = cur->next;
        }
        cout.flush();

        SFArchive in(READING_ARCHIVE);
        if (in.Lock("./file.bin", binaryReadOnly, LOCK_WAIT)) {
            leader.Serialize(in);
            in.Release();
        }

        cout << "\nRead in data\n" << SFString('-', 80) << "\n";
        cur = &leader;
        while (cur) {
            cout << cur->name << " : " << cur->age << "\n";
            cur = cur->next;
        }
        cout.flush();

        CPerson::registerClass();
        cout << leader.Format() << "\n";

        remove("./file.bin");
    }
    return 0;
}
