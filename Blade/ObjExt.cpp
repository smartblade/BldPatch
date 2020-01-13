
#include <bld_system.h>
#include <bld_python.h>
#include <raster_device.h>
#include "App.h"
#include "bld_misc_funcs.h"
#define BUILD_LIB
#include <blade_ext.h>
#include <math.h>


/*
* Module:                 Blade.exe
* Entry point:            0x00427880
*/

int SetAppMode(const char *mode)
{
    B_App * App = get_application();
    int code;
    assert(App);
    code = App->SetAppMode(mode);
    return code;
}


/*
* Module:                 Blade.exe
* Entry point:            0x004278D5
*/

const char *GetAppMode()
{
    B_App * App = get_application();
    assert(App);
    return App->GetAppMode().String();
}


/*
* Module:                 Blade.exe
* Entry point:            0x00427910
*/

void BeginLoadGame()
{
    B_App * App = get_application();
    assert(App);
    App->BeginLoadGame();
}


/*
* Module:                 Blade.exe
* Entry point:            0x00427943
*/

void DoneLoadGame()
{
    B_App * App = get_application();
    assert(App);
    App->DoneLoadGame();
}


/*
* Module:                 Blade.exe
* Entry point:            0x00427976
*/

int SetSaveInfo(PyObject *info)
{
    int saveInfoFlag;
    PyObject *data;
    int nextEntitySuffix;
    if (!PyArg_ParseTuple(info, "iO", &saveInfoFlag, &data))
        return 0;
    if (saveInfoFlag != 1)
        return 0;
    if (!PyArg_ParseTuple(data, "i", &nextEntitySuffix))
        return 0;
    B_world.nextEntitySuffix = nextEntitySuffix;
    return 0;
}


/*
* Module:                 Blade.exe
* Entry point:            0x004279E1
*/

PyObject *GetSaveInfo()
{
    PyObject *info;
    int nextEntitySuffix;
    nextEntitySuffix = B_world.nextEntitySuffix;
    info = Py_BuildValue("(i,(i))", 1, nextEntitySuffix);
    return info;
}


/*
* Module:                 Blade.exe
* Entry point:            0x00427A0D
*/

int SaveEntitiesData(const char *filename)
{
    return B_world.SaveEntitiesData(filename);
}


/*
* Module:                 Blade.exe
* Entry point:            0x00427A20
*/

int LoadEntitiesData(const char *filename)
{
    return B_world.LoadEntitiesData(filename);
}


/*
* Module:                 Blade.exe
* Entry point:            0x00427A33
*/

int SaveParticleSystemsData(const char *filename)
{
    B_ODataFile file(filename, O_BINARY);
    if (!file.OK())
        return 0;
    file << gbl_particle_types.size;
    for (unsigned int i = 0; i < gbl_particle_types.size; i++)
    {
        file << *gbl_particle_types.elements[i];
    }
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x00427B03
*/

int LoadParticleSystemsData(const char *filename)
{
    B_IDataFile file(filename, O_BINARY);
    if (!file.OK())
        return 0;
    if (gbl_particle_types.num_alloc != 0)
    {
            for (unsigned int i = 0; i < gbl_particle_types.size; i++)
            {
                    B_ParticleGType * p_type = gbl_particle_types.elements[i];
                    delete p_type;
            }
            delete(gbl_particle_types.elements);
            gbl_particle_types.elements = NULL;
            gbl_particle_types.size = 0;
            gbl_particle_types.num_alloc = 0;
    }
    file >> gbl_particle_types.size;
    gbl_particle_types.num_alloc = ((gbl_particle_types.increment + gbl_particle_types.size - 1) / gbl_particle_types.increment) * gbl_particle_types.increment;
    if (gbl_particle_types.num_alloc != 0)
    {
        gbl_particle_types.elements = new B_ParticleGType * [gbl_particle_types.num_alloc];
        for(unsigned int i = 0; i < gbl_particle_types.size; i++)
        {
            gbl_particle_types.elements[i] = new B_ParticleGType();
            file >> *gbl_particle_types.elements[i];
        }
    }
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x00427DE8
*/

int SaveCombustionData(const char *file_name)
{
    B_ODataFile file(file_name, O_BINARY);
    if (!file.OK())
        return 0;
    file << gbl_combustion_data.size;
    for (unsigned int i = 0; i < gbl_combustion_data.size; i++)
    {
        file << *gbl_combustion_data.elements[i];
    }
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x00427EB7
*/

int LoadCombustionData(const char *file_name)
{
    B_IDataFile file(file_name, O_BINARY);
    if (!file.OK())
        return 0;
    if (gbl_combustion_data.num_alloc != 0)
    {
            for (unsigned int i = 0; i < gbl_combustion_data.size; i++)
            {
                    B_Combustion * comb = gbl_combustion_data.elements[i];
                    delete comb;
            }
            delete(gbl_combustion_data.elements);
            gbl_combustion_data.elements = NULL;
            gbl_combustion_data.size = 0;
            gbl_combustion_data.num_alloc = 0;
    }
    file >> gbl_combustion_data.size;
    gbl_combustion_data.num_alloc = ((gbl_combustion_data.increment + gbl_combustion_data.size - 1) / gbl_combustion_data.increment) * gbl_combustion_data.increment;
    if (gbl_combustion_data.num_alloc != 0)
    {
        gbl_combustion_data.elements = new B_Combustion * [gbl_combustion_data.num_alloc];
        for(unsigned int i = 0; i < gbl_combustion_data.size; i++)
        {
            gbl_combustion_data.elements[i] = new B_Combustion();
            file >> *gbl_combustion_data.elements[i];
        }
    }
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042819A
*/

int ReassignCombustionData()
{
    B_App * App = get_application();
    assert(App);
    return App->ReassignCombustionData();
}


/*
* Module:                 Blade.exe
* Entry point:            0x004281D3
*/
#ifdef BLD_NATIVE
int SaveAnmRaceData(const char *file_name, const char *race)
{
    int (*bld_proc)(const char *file_name, const char *race);
    return bld_proc(file_name, race);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042857B
*/
#ifdef BLD_NATIVE
int LoadAnmRaceData(const char *file_name)
{
    int (*bld_proc)(const char *file_name);
    return bld_proc(file_name);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x004289E0
*/

int GetnRaces()
{
    return gbl_races.size;
}


/*
* Module:                 Blade.exe
* Entry point:            0x004289EA
*/

const char *GetRaceName(int index)
{
    if (index >= 0 && (unsigned int)index < gbl_races.size)
    {
        B_Race *race = gbl_races.elements[index];
        return race->Id().String();
    }
    return NULL;
}


/*
* Module:                 Blade.exe
* Entry point:            0x00428A22
*/
#ifdef BLD_NATIVE
int SaveAnmSoundData(const char *file_name, const char *race)
{
    int (*bld_proc)(const char *file_name, const char *race);
    return bld_proc(file_name, race);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x00428BEE
*/
#ifdef BLD_NATIVE
int LoadAnmSoundData(const char *file_name)
{
    int (*bld_proc)(const char *file_name);
    return bld_proc(file_name);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x00428FD6
*/

const char *GetCommandLine()
{
    B_App *App = get_application();
    assert(App);
    return App->GetCmdLine();
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042900C
*/

const char *GetCurrentMap(void)
{
    B_App *App = get_application();
    assert(App);
    return App->mapName.String();
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429049
*/

int SetCurrentMap(const char *map)
{
    B_App *App = get_application();
    assert(App);
    return App->SetCurrentMap(map);
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429086
*/

int LoadWorld(const char *file_name)
{
    B_App * app = get_application();
    assert(app);
    return app->LoadWorld(file_name);
}


/*
* Module:                 Blade.exe
* Entry point:            0x004290C0
*/

int SetListenerMode(int mode, double x, double y, double z)
{
    B_App *App = get_application();
    assert(App);
    B_Vector v;
    v.x = x;
    v.y = y;
    v.z = z;
    return App->SetListenerMode(mode, v);
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429128
*/

int GetListenerMode()
{
    B_App *App = get_application();
    assert(App);
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042915B
*/

int Quit()
{
    B_App *App = get_application();
    assert(App);
    return App->Quit();
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429194
*/

int SetTime(double time)
{
    B_App *App = get_application();
    assert(App);
    return App->SetTime(time);
}


/*
* Module:                 Blade.exe
* Entry point:            0x004291D2
*/

int GoToTime(double time)
{
    B_App *App = get_application();
    assert(App);
    return App->GoToTime(time);
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429210
*/

double GetTime()
{
    return B_world.get_time();
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042921F
*/

void StopTime()
{
    B_App *App = get_application();
    assert(App);
    return App->StopTime();
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429255
*/

void RestartTime()
{
    B_App *App = get_application();
    assert(App);
    App->RestartTime();
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042928B
*/

void SetTimeSpeed(double speed)
{
    B_App *App = get_application();
    assert(App);
    App->SetTimeSpeed(speed);
}


/*
* Module:                 Blade.exe
* Entry point:            0x004292C9
*/

double GetTimeSpeed()
{
    B_App *App = get_application();
    assert(App);
    return App->GetTimeSpeed();
}


/*
* Module:                 Blade.exe
* Entry point:            0x004292FF
*/

int AddInputAction(const char *action_name, int npi)
{
    B_App *App = get_application();
    assert(App);
    App->AddInputAction(action_name, npi);
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429342
*/

int RemoveInputAction(const char *action_name)
{
    B_App *App = get_application();
    assert(App);
    App->RemoveInputAction(action_name);
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429381
*/

int AssocKey(
    const char *action, const char *input_device, const char *key,
    int press
)
{
    B_App *App = get_application();
    assert(App);
    return App->AssocKey(action, input_device, key, press);
}


/*
* Module:                 Blade.exe
* Entry point:            0x004293C7
*/

int Bind(const char *key, PyObject *proc)
{
    assert(key);
    assert(proc);
    B_App *App = get_application();
    assert(App);
    return App->Bind(key, proc);
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429446
*/

int UnBind(const char *key, PyObject *proc)
{
    assert(key);
    assert(proc);
    B_App *App = get_application();
    assert(App);
    return App->UnBind(key, proc);
}


/*
* Module:                 Blade.exe
* Entry point:            0x004294C5
*/

int Bind2(
    const char *act, const char *subact1,
    const char *subact2, int unknown
)
{
    B_App *App = get_application();
    assert(App);
    assert(act);
    assert(subact1);
    assert(subact2);
    App->Bind2(act, subact1, subact2, unknown);
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042956A
*/

double GetTimeActionHeld(const char *Action)
{
    assert(Action);
    B_App *App = get_application();
    assert(App);
    return App->GetTimeActionHeld(Action);
}


/*
* Module:                 Blade.exe
* Entry point:            0x004295C2
*/

int BindPred(const char *key, const char *pred)
{
    assert(key);
    assert(pred);
    B_App *App = get_application();
    assert(App);
    return App->BindPred(key, pred);
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429641
*/

int UnBindPred(const char *key, const char *pred)
{
    assert(key);
    assert(pred);
    B_App *App = get_application();
    assert(App);
    return App->UnBindPred(key, pred);
}


/*
* Module:                 Blade.exe
* Entry point:            0x004296C0
*/

void UnBindAll()
{
    B_App *App = get_application();
    assert(App);
    App->UnBindAll();
}


/*
* Module:                 Blade.exe
* Entry point:            0x004296F6
*/

const char *Input(const char *text)
{
    B_App *App = get_application();
    assert(App);
    return App->Input(text);
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429733
*/

int MarkLevelToLoad(const char *filename)
{
    assert(filename);
    B_App *App = get_application();
    assert(App);
    App->MarkLevelToLoad(filename);
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429793
*/

int ReadLevel(const char *filename)
{
    assert(filename);
    B_App *App = get_application();
    assert(App);
    App->ReadLevel(filename);
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x004297F3
*/

unsigned int ReadBitMap(const char *filename, const char *internalname)
{
    assert(filename);
    return B_resource_manager.LocateResourceIn(filename, &B_Name(internalname), 3);
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429863
*/

unsigned int ReadAlphaBitMap(const char *filename, const char *internalname)
{
    assert(filename);
    return B_resource_manager.LocateResourceIn(filename, &B_Name(internalname), 4);
}


/*
* Module:                 Blade.exe
* Entry point:            0x004298D3
*/

void CloseLevel(const char *statement, const char *auxText)
{
    B_App *App = get_application();
    assert(App);
    App->CloseLevel(statement, auxText);
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429914
*/

int StartRecordingDemo()
{
    return gbl_en_control.StartRecordingDemo();
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429923
*/

int EndRecordingDemo(const char *name)
{
    return gbl_en_control.EndRecordingDemo(name);
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429936
*/

int PlayDemo(const char *name)
{
    return gbl_en_control.PlayDemo(name);
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429949
*/

int DeactivateInput()
{
    B_App *App = get_application();
    assert(App);
    return App->DeactivateInput();
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042997F
*/

int ActivateInput()
{
    B_App *App = get_application();
    assert(App);
    return App->ActivateInput();
}


/*
* Module:                 Blade.exe
* Entry point:            0x004299B5
*/

int OutSound(int soundId, double x, double y, double z, int unknown)
{
    B_App *App = get_application();
    assert(App);
    return App->OutSound(soundId, x, y, z, unknown);
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429A10
*/

int SetDefaultMass(const char *entity_kind, double mass)
{
    B_ElementCSV *foundElement;
    B_Name kind(entity_kind);
    B_NDataBase<B_ElementCSV> * array = &B_csvs;
    for (unsigned int i = 0; i < array->size; i++)
    {
        if (kind == array->elements[i]->Id())
        {
            foundElement = array->elements[i];
            goto after_search;
        }
    }
    foundElement = NULL;

after_search:
    if (foundElement)
    {
        foundElement->mass = mass;
        return 1;
    }
    B_ElementCSV * newElement = new B_ElementCSV();
    newElement->name = entity_kind;
    newElement->mass = mass;
    newElement->material = "";
    B_NDataBase<B_ElementCSV> * csvs = &B_csvs;
    if (csvs->num_alloc > csvs->size)
    {
        csvs->elements[csvs->size] = newElement;
        csvs->size++;
    }
    else
    {
        csvs->num_alloc += csvs->increment;
        if (csvs->size != 0)
        {
            B_ElementCSV ** elements = new B_ElementCSV *[csvs->num_alloc];
            for (unsigned int i = 0; i < csvs->size; i++)
            {
                elements[i] = csvs->elements[i];
            }
            delete csvs->elements;
            csvs->elements = elements;
        }
        else
        {
            csvs->elements = new B_ElementCSV *[csvs->num_alloc];
        }
        csvs->elements[csvs->size] = newElement;
        csvs->size++;
    }
    return 0;
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429C4F
*/

int SetDefaultMaterial(const char *entity_kind, const char *material)
{
    B_ElementCSV *foundElement;
    B_Name kind(entity_kind);
    B_NDataBase<B_ElementCSV> * array = &B_csvs;
    for (unsigned int i = 0; i < array->size; i++)
    {
        if (kind == array->elements[i]->Id())
        {
            foundElement = array->elements[i];
            goto after_search;
        }
    }
    foundElement = NULL;

after_search:
    if (foundElement)
    {
        foundElement->material = material;
        return 1;
    }
    B_ElementCSV * newElement = new B_ElementCSV();
    newElement->name = entity_kind;
    newElement->mass = 10.0;
    newElement->material = material;
    B_NDataBase<B_ElementCSV> * csvs = &B_csvs;
    if (csvs->num_alloc > csvs->size)
    {
        csvs->elements[csvs->size] = newElement;
        csvs->size++;
    }
    else
    {
        csvs->num_alloc += csvs->increment;
        if (csvs->size != 0)
        {
            B_ElementCSV ** elements = new B_ElementCSV *[csvs->num_alloc];
            for (unsigned int i = 0; i < csvs->size; i++)
            {
                elements[i] = csvs->elements[i];
            }
            delete csvs->elements;
            csvs->elements = elements;
        }
        else
        {
            csvs->elements = new B_ElementCSV *[csvs->num_alloc];
        }
        csvs->elements[csvs->size] = newElement;
        csvs->size++;
    }
    return 0;
}


/*
................................................................................
................................................................................
................................................................................
................................................................................
*/

/*
* Module:                 Blade.exe
* Entry point:            0x00429F2D
*/

void SetSun(int exists, double x, double y, double z)
{
    B_Vector sunPosition;
    sunPosition.x = x;
    sunPosition.y = y;
    sunPosition.z = z;
    B_world.SetSun(exists, sunPosition);
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429F71
*/

int InsideAA(int AA, double x, double y, double z)
{
    B_Sector *sector;
    int sectorIndex;
    B_Vector point;
    point.x = x;
    point.y = y;
    point.z = z;
    sectorIndex = B_world.FindSectorIndex(point);
    if (sectorIndex == -1)
        return 0;
    sector = B_world.sectors.elements[sectorIndex];
    return sector->unknown204.InsideAA(AA);
}


/*
* Module:                 Blade.exe
* Entry point:            0x00429FDF
*/

int SetDrawObjectShadows(int draw)
{
    B_App *App = get_application();
    App->drawShadows = draw;
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A000
*/

int GetDrawObjectShadows()
{
    int drawShadows = get_application()->drawShadows;
    return drawShadows;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A019
*/

int SetAutoEngageCombat(int auto_engage_combat)
{
    B_App *App = get_application();
    App->autoEngageCombat = auto_engage_combat;
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A03A
*/

int GetAutoEngageCombat()
{
    int autoEngageCombat = get_application()->autoEngageCombat;
    return autoEngageCombat;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A053
*/

int SetAecGap(double aec_gap)
{
    B_App *App = get_application();
    App->aecGap = aec_gap;
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A07D
*/

double GetAecGap()
{
    double aecGap = get_application()->aecGap;
    return aecGap;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A095
*/

int SetBloodLevel(int blood_level)
{
    B_App *App = get_application();
    assert(App);
    return App->SetBloodLevel(blood_level);
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A0CF
*/

int GetBloodLevel()
{
    B_App *App = get_application();
    assert(App);
    return App->GetBloodLevel();
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A105
*/

int SetMutilationLevel(int mutilation_level)
{
    B_App *App = get_application();
    assert(App);
    return App->SetMutilationLevel(mutilation_level);
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A13F
*/

int GetMutilationLevel()
{
    B_App *App = get_application();
    assert(App);
    return App->GetMutilationLevel();
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A175
*/

void AnmAddEvent(const char *anm_name, const char *event_name, double event_frame)
{
    anim_t *foundAnim;
    B_Name name(anm_name);
    for (unsigned int i = 0; i < gbl_anims.size; i++)
    {
        if (name == gbl_anims.elements[i]->Id())
        {
            foundAnim = gbl_anims.elements[i];
            goto after_search;
        }
    }
    foundAnim = NULL;

after_search:
    if (foundAnim == NULL)
    {
        foundAnim = LoadFromHDAnim(anm_name);
    }
    if (foundAnim != NULL)
    {
        foundAnim->AddEvent(event_name, event_frame);
    }
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A232
*/

void AnmDelEvent(const char *anm_name, const char *event_name)
{
    anim_t *foundAnim;
    B_Name name(anm_name);
    for (unsigned int i = 0; i < gbl_anims.size; i++)
    {
        if (name == gbl_anims.elements[i]->Id())
        {
            foundAnim = gbl_anims.elements[i];
            goto after_search;
        }
    }
    foundAnim = NULL;

after_search:
    if (foundAnim != NULL)
    {
        foundAnim->DelEvent(event_name);
    }
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A2D0
*/

void AnmClearEvents(const char *anm_name)
{
    anim_t *foundAnim;
    B_Name name(anm_name);
    for (unsigned int i = 0; i < gbl_anims.size; i++)
    {
        if (name == gbl_anims.elements[i]->Id())
        {
            foundAnim = gbl_anims.elements[i];
            goto after_search;
        }
    }
    foundAnim = NULL;

after_search:
    if (foundAnim != NULL)
    {
        foundAnim->ClearEvents();
    }
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A354
*/

double AnmGetEventFrame(const char *anm_name, const char *event_name)
{
    anim_t *foundAnim;
    B_Name name(anm_name);
    for (unsigned int i = 0; i < gbl_anims.size; i++)
    {
        if (name == gbl_anims.elements[i]->Id())
        {
            foundAnim = gbl_anims.elements[i];
            goto after_search;
        }
    }
    foundAnim = NULL;

after_search:
    if (foundAnim != NULL)
        return foundAnim->GetEventFrame(event_name);
    return -1.0;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A400
*/

int AddParticleGType(
    const char *new_type, const char *parent_type, int operation_type,
    int duration
)
{
    bool found;
    {
        int foundIndex;
        B_Name name(new_type);
        for (unsigned int i = 0; i < gbl_particle_types.size; i++)
        {
            if (name == gbl_particle_types.elements[i]->Id())
            {
                foundIndex = i;
                goto after_search;
            }
        }
        foundIndex = -1;

after_search:
        found = (foundIndex != -1);
    }
    if (found)
        return 1;
    B_ParticleGType *newParticleType = new B_ParticleGType(
        new_type,
        parent_type,
        operation_type,
        duration);
    if (newParticleType == NULL)
        return 0;
    for (int i = 0; i < duration + 1; i++)
    {
        newParticleType->a00C.elements[i].r = 128;
        newParticleType->a00C.elements[i].g = 128;
        newParticleType->a00C.elements[i].b = 128;
        newParticleType->a00C.elements[i].alpha = 128;
        newParticleType->a00C.elements[i].size = 100.0f;
    }
    if (gbl_particle_types.num_alloc > gbl_particle_types.size)
    {
        gbl_particle_types.elements[gbl_particle_types.size] = newParticleType;
        gbl_particle_types.size++;
    }
    else
    {
        gbl_particle_types.num_alloc += gbl_particle_types.increment;
        if (gbl_particle_types.size != 0)
        {
            B_ParticleGType ** elements = new B_ParticleGType *[gbl_particle_types.num_alloc];
            for (unsigned int i = 0; i < gbl_particle_types.size; i++)
            {
                elements[i] = gbl_particle_types.elements[i];
            }
            delete gbl_particle_types.elements;
            gbl_particle_types.elements = elements;
        }
        else
        {
            gbl_particle_types.elements = new B_ParticleGType *[gbl_particle_types.num_alloc];
        }
        gbl_particle_types.elements[gbl_particle_types.size] = newParticleType;
        gbl_particle_types.size++;
    }
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A697
*/

int SetParticleGVal(
    const char *type, int i, int r, int g, int b, int alpha, double size
)
{
    B_ParticleGType *foundElement;
    {
        B_Name name(type);
        for (unsigned int i = 0; i < gbl_particle_types.size; i++)
        {
            if (name == gbl_particle_types.elements[i]->Id())
            {
                foundElement = gbl_particle_types.elements[i];
                goto after_search;
            }
        }
        foundElement = NULL;
after_search:
        ;
    }
    if (foundElement != NULL && (unsigned int)i < foundElement->a00C.size)
    {
        foundElement->a00C.elements[i].r = r;
        foundElement->a00C.elements[i].g = g;
        foundElement->a00C.elements[i].b = b;
        foundElement->a00C.elements[i].alpha = alpha;
        foundElement->a00C.elements[i].size = size;
    }
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A775
*/

int GetnParticleGType()
{
    return gbl_particle_types.size;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A77F
*/

int GetParticleGType(
    int index, const char **type, const char **parent_type,
    int *operation_type, int *duration
)
{
    if (index >= 0 && (unsigned int)index < gbl_particle_types.size)
    {
        B_ParticleGType *particleType = gbl_particle_types.elements[index];
        *type = particleType->name.String();
        *parent_type = B_3D_raster_device->bmp_name(particleType->bmp_handle);
        *operation_type = particleType->operation_type;
        *duration = particleType->duration;
        return 1;
    }
    return 0;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A82F
*/

int GetParticleGVal(
    const char *type, int index, int *r, int *g, int *b, int *alpha,
    double *size
)
{
    B_ParticleGType *foundElement;
    {
        B_Name name(type);
        for (unsigned int i = 0; i < gbl_particle_types.size; i++)
        {
            if (name == gbl_particle_types.elements[i]->Id())
            {
                foundElement = gbl_particle_types.elements[i];
                goto after_search;
            }
        }
        foundElement = NULL;
after_search:
        ;
    }
    if (foundElement != NULL)
    {
        *r = foundElement->a00C.elements[index].r;
        *g = foundElement->a00C.elements[index].g;
        *b = foundElement->a00C.elements[index].b;
        *alpha = foundElement->a00C.elements[index].alpha;
        *size = foundElement->a00C.elements[index].size;
        return 1;
    }
    return 0;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A915
*/

int SetTurnSpeed(const char *race_name, double new_speed)
{
    return gbl_en_control.SetTurnSpeed(race_name, new_speed);
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A930
*/

int SetAnimationFactor(const char *mov, double new_speed_factor)
{
    return gbl_en_control.SetAnimationFactor(mov, new_speed_factor);
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A94B
*/

int SetAutoGenTexture(const char *textureName, int textureEffect)
{
    B_App *App = get_application();
    assert(App);
    App->SetAutoGenTexture(textureName, textureEffect);
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A98E
*/

const char *GenerateEntityName()
{
    return B_world.GenerateEntityName("Entity");
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A9A2
*/

int AddWatchAnim(const char *anim)
{
    return gbl_en_control.AddWatchAnim(anim);
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A9B5
*/

int AddAnimFlags(
    const char *anim, int wuea, int mdf_y, int solf, int copy_rot,
    int bng_mov, int head_f
)
{
    return gbl_en_control.AddAnimFlags(
        anim, wuea, mdf_y, solf, copy_rot, bng_mov, head_f);
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042A9E0
*/

int SetActionCameraMovement(
    const char *action_name, double angle, double start_pos, double end_pos
)
{
    B_App *App = get_application();
    assert(App);
    B_CameraEntity *camera = App->camera;
    if (camera != NULL)
    {
        B_CameraMovement *movement = new B_CameraMovement(
            action_name, angle, start_pos, end_pos);
        array_t<B_CameraMovement *> *array = &camera->movements;
        if (array->num_alloc > array->size)
        {
            array->elements[array->size] = movement;
            array->size++;
        }
        else
        {
            array->num_alloc += array->increment;
            if (array->size != 0)
            {
                B_CameraMovement ** elements = new B_CameraMovement *[array->num_alloc];
                for (unsigned int i = 0; i < array->size; i++)
                {
                    elements[i] = array->elements[i];
                }
                delete array->elements;
                array->elements = elements;
            }
            else
            {
                array->elements = new B_CameraMovement *[array->num_alloc];
            }
            array->elements[array->size] = movement;
            array->size++;
        }
    }
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042ABBA
*/

int SetRootWidget(long int widget)
{
    B_App *App = get_application();
    assert(App);
    App->rootWidget = widget;
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042ABF6
*/

long int GetRootWidget()
{
    B_App *App = get_application();
    assert(App);
    long int rootWidget = App->rootWidget;
    return rootWidget;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042AC32
*/

int AddCombustionDataFor(
        const char *object_kind, const char *fire_kind, double upper_treshol,
        double lower_treshold, double flame_height, double flame_size,
        double speed, double livetime
)
{
    bool notFound;
    {
        B_Combustion *foundElement;
        B_Name name(object_kind);
        for (unsigned int i = 0; i < gbl_combustion_data.size; i++)
        {
            if (name == gbl_combustion_data.elements[i]->Id())
            {
                foundElement = gbl_combustion_data.elements[i];
                goto after_search;
            }
        }
        foundElement = NULL;
after_search:
        notFound = (foundElement == NULL);
    }
    if (notFound)
    {
        B_Combustion *newCombustion = new B_Combustion(
            object_kind, fire_kind, upper_treshol, lower_treshold,
            flame_height, flame_size, speed, livetime);
        array_t<B_Combustion *> *array = &gbl_combustion_data;
        if (array->num_alloc > array->size)
        {
            array->elements[array->size] = newCombustion;
            array->size++;
        }
        else
        {
            array->num_alloc += array->increment;
            if (array->size != 0)
            {
                B_Combustion ** elements = new B_Combustion *[array->num_alloc];
                for (unsigned int i = 0; i < array->size; i++)
                {
                    elements[i] = array->elements[i];
                }
                delete array->elements;
                array->elements = elements;
            }
            else
            {
                array->elements = new B_Combustion *[array->num_alloc];
            }
            array->elements[array->size] = newCombustion;
            array->size++;
        }
        return 1;
    }
    return 0;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042AE7A
*/

void RM_FreeResource(B_Resource *resource)
{
    delete resource;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042AEAE
*/

B_BitMap *RM_GetResource(const char *name)
{
    B_Resource *resource;
    resource = B_resource_manager.GetResource(2, name);
    if (resource != NULL)
    {
        B_BitMap *bitMap = new B_BitMap((B_BitMap &)resource->data);
        B_resource_manager.FreeResource(resource);
        return bitMap;
    }
    resource = B_resource_manager.GetResource(3, name);
    if (resource != NULL)
    {
        B_BitMap24 *bitMap24 = (B_BitMap24 *)&resource->data;
        B_BitMap *bitMap = new B_BitMap(
            B_BitMap::BitMapType4, bitMap24->dimension1, bitMap24->dimension2,
            NULL);
        memcpy(
            bitMap->data, bitMap24->data,
            bitMap->dimension1 * bitMap->dimension2 * 3);
        B_resource_manager.FreeResource(resource);
        return bitMap;
    }
    resource = B_resource_manager.GetResource(4, name);
    if (resource != NULL)
    {
        B_BitMap24 *bitMap24 = (B_BitMap24 *)&resource->data;
        B_BitMap *bitMap = new B_BitMap(
            B_BitMap::BitMapType2, bitMap24->dimension1, bitMap24->dimension2,
            NULL);
        for(unsigned int i = 0; i < bitMap->dimension1 * bitMap->dimension2; i++)
        {
            ((byte *)bitMap->data)[i] = ((byte *)bitMap24->data)[i * 3];
        }
        B_resource_manager.FreeResource(resource);
        return bitMap;
    }
    return NULL;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B0F4
*/

void UpdateAGTexture(long bmp_handle)
{
    B_AGTexture *texture;
    {
        for (unsigned int i = 0; i < gbl_ag_textures.size; i++)
        {
            B_AGTexture *currentTexture = gbl_ag_textures.elements[i];
            long currentHandle = currentTexture->bmp_handle;
            if (currentHandle == bmp_handle)
            {
                B_AGTexture *foundTexture = gbl_ag_textures.elements[i];
                texture = foundTexture;
                goto after_search;
            }
        }
        texture = NULL;
after_search:
        ;
    }
    if (
        (texture != NULL) &&
        (texture->procTexture->data != NULL) &&
        (texture->needUpdate))
    {
        texture->procTexture->Update();
        texture->needUpdate = 0;
    }
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B192
*/

const char *GetWorldFileName(void)
{
    return B_world.world_file_name;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B1A5
*/

int SetAfterFrameFunc(const char *name, PyObject *function)
{
    B_App *App = get_application();
    assert(App);
    App->SetAfterFrameFunc(name, function);
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B1E8
*/

int RemoveAfterFrameFunc(const char *name)
{
    B_App *App = get_application();
    assert(App);
    App->RemoveAfterFrameFunc(name);
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B227
*/

PyObject *GetAfterFrameFunc(const char *name)
{
    B_App *App = get_application();
    assert(App);
    return App->GetAfterFrameFunc(name);
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B261
*/

const char *GetAfterFrameFuncName(int index)
{
    B_App *App = get_application();
    assert(App);
    return App->GetAfterFrameFuncName(index);
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B29B
*/

int GetnAfterFrameFuncs()
{
    B_App *App = get_application();
    assert(App);
    return App->GetnAfterFrameFuncs();
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B2D1
*/

int SetMenuTgapFunc(PyObject *func)
{
    B_App *App = get_application();
    assert(App);
    App->SetMenuTgapFunc(func);
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B310
*/

PyObject *GetMenuTgapFunc()
{
    B_App *App = get_application();
    assert(App);
    return App->GetMenuTgapFunc();
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B346
*/

int SetCallCheck(int check)
{
    int checkErrorsPrev = CheckErrors;
    CheckErrors = check;
    return checkErrorsPrev;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B366
*/

int OpenDebugChannel(const char *channel_name)
{
    int result = mout.OpenChannel(channel_name);
    return result;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B39C
*/

int CloseDebugChannel(const char *channel_name)
{
    int result = mout.CloseChannel(channel_name);
    return result;
}


/*
................................................................................
................................................................................
................................................................................
................................................................................
*/

/*
* Module:                 Blade.exe
* Entry point:            0x0042B444
*/

int SetShowAreas(int action_areas_bit_mask)
{
    gbl_action_areas_bit_mask = action_areas_bit_mask;
    return 0;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B453
*/

int WriteText(double x, double y, const char *text)
{
    int w, h;
    B_3D_raster_device->get_size(w, h);
    B_3D_raster_device->set_position((w / 2) + w * x, (h / 2) + w * y);
    B_3D_raster_device->write_text(text);
    return 0;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B4EC
*/

int GetTextWH(const char *text, double *w, double *h)
{
    *w = 0;
    *h = 0;
    B_Name txt(text);
    B_Font *font = B_3D_raster_device->GetFont();
    if (font == NULL)
    {
        mout << "WARNING: GetTextWH fails because Font can't be found.";
        return 0;
    }
    B_CharData *charData = font->charData;
    if (font == NULL)/*FIXME should be charData*/
    {
        mout << "TERRIBLE WARNING: GetTextWH fails because CharData can't be found.";
        return 0;
    }
    for(unsigned int i = 0; i < txt.Length(); i++)
    {
        double height;
        *w += charData->charSize[text[i]].width;
        if (charData->charSize[text[i]].height < *h)
        {
            height = *h;
        }
        else
        {
            height = charData->charSize[text[i]].height;
        }
        *h = height;
    }
    *w /= 640.0;
    *h /= 640.0;
    return 0;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B672
*/

int GetScreenRect(
    double *x_min, double *y_min, double *x_max, double *y_max
)
{
    Unknown004CD5EC unknown(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    float coef1 = 1.0f;
    float coef2 = 1.0f;
    unknown.unknown000 = (1.0f - coef1) * 0.5f + 0.001f;
    unknown.unknown004 = (1.0f - coef1) * 0.5f * 0.75f + 0.001f;
    unknown.unknown008 = 0.998f * coef1;
    unknown.unknown00C = 0.748f * coef1;
    unknown.unknown010 = coef2;
    unknown.unknown014 = 0.65 * coef1;
    unknown.unknown004CD634();
    *x_min = unknown.unknown000 - unknown.unknown008 * 0.5;
    *y_min = unknown.unknown004 - unknown.unknown00C * 0.5;
    *x_max = unknown.unknown000 + unknown.unknown008 * 0.5;
    *y_max = unknown.unknown004 + unknown.unknown00C * 0.5;
    int result = 0;
    return result;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B795
*/

int GetScreenXY(
    double map_x, double map_y, double map_z, double *screen_x,
    double *screen_y
)
{
    Unknown004CD5EC unknown(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    float coef1 = 1.0f;
    float coef2 = 1.0f;
    unknown.unknown000 = (1.0f - coef1) * 0.5f + 0.001f;
    unknown.unknown004 = (1.0f - coef1) * 0.5f * 0.75f + 0.001f;
    unknown.unknown008 = 0.998f * coef1;
    unknown.unknown00C = 0.748f * coef1;
    unknown.unknown010 = coef2;
    unknown.unknown014 = 0.65 * coef1;
    unknown.unknown004CD634();
    B_Vector map_point;
    map_point.x = map_x;
    map_point.y = map_y;
    map_point.z = map_z;
    B_Vector screenPoint = map_point * get_application()->location.matrix0030;
    if (fabs(screenPoint.z) < 0.01)
        screenPoint.z = 0.01;
    double factor = unknown.unknown014 / screenPoint.z;
    *screen_x = screenPoint.x * factor;
    *screen_y = screenPoint.y * factor;
    int result = 0;
    return result;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B8E8
*/

int DrawBOD(
    const char *BODName, int x, int y, int w, int h, double scale,
    int i_unknown
)
{
    B_ObjDscr *bod = (B_ObjDscr *)B_resource_manager.GetResource(1, BODName);
    if (bod == NULL)
        return 0;
    bod->Draw(B_3D_raster_device, x, y, (x + w), (y + h), scale, i_unknown);
    return 1;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042B97A
*/

int CreateTriggerSector(
    const char *trigger_sector_name, const char *group_name,
    double floor_height, double roof_height, int num_points,
    point_2d_t *points
)
{
    B_TriggerSector *TS = new B_TriggerSector(
        trigger_sector_name, group_name, floor_height, roof_height);
    if (TS == NULL)
        return 0;
    for (int i = 0; i < num_points; i++)
    {
        double y = points[i].y;
        double x = points[i].x;
        B_Vector point;
        point.x = x;
        point.y = floor_height;
        point.z = y;
        TS->AddPoint(point, num_points);
    }
    assert(TS);
    char * name = TS->Id().String();
    assert(name);

    bool found;
    {
        B_TriggerSector *foundElement;
        B_Name tsName(name);
        for (unsigned int i = 0; i < B_world.triggerSectors.size; i++)
        {
            if (tsName == B_world.triggerSectors.elements[i]->Id())
            {
                foundElement = B_world.triggerSectors.elements[i];
                goto after_search;
            }
        }
        foundElement = NULL;
after_search:
        found = (foundElement != NULL);
    }
    int code;
    if (!found)
    {
        B_world.triggerSectors.addElement(TS);
        code = 1;
    }
    else
    {
        /* FIXME memory leak (TS was not deleted) */
        code = 0;
    }
    return code;
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042BC9A
*/

int SetTriggerSectorFunc(
    const char *trigger_sector_name, const char *func_type, PyObject *func
)
{
    return B_world.SetTriggerSectorFunc(trigger_sector_name, func_type, func);
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042BCB5
*/

PyObject *GetTriggerSectorFunc(
    const char *trigger_sector_name, const char *func_type
)
{
    return B_world.GetTriggerSectorFunc(trigger_sector_name, func_type);
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042BCCC
*/

int GetTriggerSectorFloorHeight(const char *trigger_sector_name)
{
    return B_world.GetTriggerSectorFloorHeight(trigger_sector_name);
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042BCDF
*/

int GetTriggerSectorRoofHeight(const char *trigger_sector_name)
{
    return B_world.GetTriggerSectorRoofHeight(trigger_sector_name);
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042BCF2
*/

const char *GetTriggerSectorGroup(const char *trigger_sector_name)
{
    return B_world.GetTriggerSectorGroup(trigger_sector_name);
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042BD05
*/

double *GetTriggerSectorPoints(const char *trigger_sector_name)
{
    return B_world.GetTriggerSectorPoints(trigger_sector_name);
}


/*
* Module:                 Blade.exe
* Entry point:            0x0042BD18
*/
#ifdef BLD_NATIVE
int SetTriggerSectorData(
        const char *trigger_sector_name, PyObject *data
)
{
    int (*bld_proc)(
        const char *trigger_sector_name, PyObject *data
);
        const char *trigger_sector_name, PyObject *data
))GetProcAddress(blade, "SetTriggerSectorData");
    return bld_proc(trigger_sector_name, data);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042BD2F
*/
#ifdef BLD_NATIVE
PyObject *GetTriggerSectorData(const char *trigger_sector_name)
{
    PyObject *(*bld_proc)(const char *trigger_sector_name);
    return bld_proc(trigger_sector_name);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042BD42
*/
#ifdef BLD_NATIVE
int nTriggerSectors(void)
{
    int (*bld_proc)(void);
    return bld_proc();
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042BD55
*/
#ifdef BLD_NATIVE
const char *GetTriggerSectorName(int index)
{
    const char *(*bld_proc)(int index);
    return bld_proc(index);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042BD68
*/
#ifdef BLD_NATIVE
int GetModelPos(const char *person, double *x, double *y, double *z)
{
    int (*bld_proc)(const char *person, double *x, double *y, double *z);
    return bld_proc(person, x, y, z);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042BFF8
*/
#ifdef BLD_NATIVE
int GetWindowId(void)
{
    int (*bld_proc)(void);
    return bld_proc();
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C031
*/
#ifdef BLD_NATIVE
int GetProgramId(void)
{
    int (*bld_proc)(void);
    return bld_proc();
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C06A
*/
#ifdef BLD_NATIVE
void TakeSnapShot(void)
{
    void (*bld_proc)(void);
    bld_proc();
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C0A5
*/
#ifdef BLD_NATIVE
int nSectors(void)
{
    int (*bld_proc)(void);
    return bld_proc();
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C0AF
*/
#ifdef BLD_NATIVE
void SetRunString(const char *variable)
{
    void (*bld_proc)(const char *variable);
    bld_proc(variable);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C0EC
*/
#ifdef BLD_NATIVE
int SetStringValue(const char *variable, const char *value)
{
    int (*bld_proc)(const char *variable, const char *value);
    return bld_proc(variable, value);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C12A
*/
#ifdef BLD_NATIVE
const char *GetStringValue(const char *variable)
{
    const char *(*bld_proc)(const char *variable);
    return bld_proc(variable);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C164
*/
#ifdef BLD_NATIVE
int DeleteStringValue(const char *variable)
{
    int (*bld_proc)(const char *variable);
    return bld_proc(variable);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C19E
*/
#ifdef BLD_NATIVE
int SaveProfileData(const char *file_name)
{
    int (*bld_proc)(const char *file_name);
    return bld_proc(file_name);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C1B9
*/
#ifdef BLD_NATIVE
int OpenProfileSection(int section, const char *comment)
{
    int (*bld_proc)(int section, const char *comment);
    return bld_proc(section, comment);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C1D4
*/
#ifdef BLD_NATIVE
int CloseProfileSection(int section)
{
    int (*bld_proc)(int section);
    return bld_proc(section);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C1EF
*/
#ifdef BLD_NATIVE
int StartProfile(void)
{
    int (*bld_proc)(void);
    return bld_proc();
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C20A
*/
#ifdef BLD_NATIVE
int EnableProfiler(void)
{
    int (*bld_proc)(void);
    return bld_proc();
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C225
*/
#ifdef BLD_NATIVE
int DisableProfiler(void)
{
    int (*bld_proc)(void);
    return bld_proc();
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C240
*/
#ifdef BLD_NATIVE
int SetInputMode(const char *device, const char *mode)
{
    int (*bld_proc)(const char *device, const char *mode);
    return bld_proc(device, mode);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C281
*/
#ifdef BLD_NATIVE
const char *GetInputMode(const char *device)
{
    const char *(*bld_proc)(const char *device);
    return bld_proc(device);
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0042C2BE
*/
#ifdef BLD_NATIVE
const char *GetLastPlayerCType(void)
{
    const char *(*bld_proc)(void);
    return bld_proc();
}
#endif
