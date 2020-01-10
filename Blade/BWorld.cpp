
#include <bld_system.h>
#include <raster_device.h>
#include "BWorld.h"
#include "light.h"
#include "sector.h"
#include "bld_misc_funcs.h"


/*
................................................................................
................................................................................
................................................................................
................................................................................
*/

/*
* Module:                 Blade.exe
* Entry point:            0x00439F5D
* VC++ mangling:          ?unknown_00439F5D@B_World@@QAEXXZ
*/

#ifdef BLD_NATIVE

void B_World::unknown_00439F5D()
{
}

#endif

/*
................................................................................
................................................................................
................................................................................
................................................................................
*/

/*
* Module:                 Blade.exe
* Entry point:            0x0043A6A4
* VC++ mangling:          ?get_time@B_World@@QAENXZ
*/

#ifdef BLD_NATIVE

double B_World::get_time()
{
}

#endif

/*
................................................................................
................................................................................
................................................................................
................................................................................
*/


/*
* Module:                 Blade.exe
* Entry point:            0x0043C8B2
* VC++ mangling:          ?SetSun@B_World@@QAEXHABVB_Vector@@@Z
*/

#ifdef BLD_NATIVE

int B_World::SetSun(int exists, const B_Vector &position)
{
}

#endif

/*
................................................................................
................................................................................
................................................................................
................................................................................
*/


/*
* Module:                 Blade.exe
* Entry point:            0x0043DBBE
* VC++ mangling:          ?FindSectorIndex@B_World@@QAEHABVB_Vector@@@Z
*/

#ifdef BLD_NATIVE

int B_World::FindSectorIndex(const B_Vector &point)
{
    return -1;
}

#endif

/*
................................................................................
................................................................................
................................................................................
................................................................................
*/


/*
* Module:                 Blade.exe
* Entry point:            0x0043E8E8
* VC++ mangling:          ?GenerateEntityName@B_World@@QAEPBDPBD@Z
*/
#ifdef BLD_NATIVE
const char *GenerateEntityName(const char *prefix)
{
    return NULL;
}
#endif

/*
................................................................................
................................................................................
................................................................................
................................................................................
*/

/*
* Module:                 Blade.exe
* Entry point:            0x000043F98A
* VC++ mangling:          ?LoadEntitiesData@B_World@@QAEHPBD@Z
*/

#ifdef BLD_NATIVE

int B_World::LoadEntitiesData(const char *filename)
{
}

#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0043FD82
* VC++ mangling:          ?SaveEntitiesData@B_World@@QAEHPBD@Z
*/

#ifdef BLD_NATIVE

int B_World::SaveEntitiesData(const char *filename)
{
}

#endif

/*
................................................................................
................................................................................
................................................................................
................................................................................
*/


/*
* Module:                 Blade.exe
* Entry point:            0x00440A5D
* VC++ mangling:          ??5@YAAAVB_IDataFile@@AAV0@PAVB_World@@@Z
*/

B_IDataFile& operator >>(B_IDataFile& file, B_World *world)
{
        unsigned int num_lights;

        world->unknown187C = 0;
        world->unknown1880 = 0;
        array_t<atmosphere_t *> *atms = &world->atmospheres;
        if (atms->num_alloc != 0)
        {
                for (unsigned int i = 0; i < atms->size; i++)
                {
                        atmosphere_t * atm = atms->elements[i];
                        delete atm;
                }
                delete(atms->elements);
                atms->elements = NULL;
                atms->size = 0;
                atms->num_alloc = 0;
        }
        file >> atms->size;
        atms->num_alloc = ((atms->increment + atms->size - 1) / atms->increment) * atms->increment;
        if (atms->num_alloc != 0)
        {
                atms->elements = new atmosphere_t * [atms->num_alloc];
                for(unsigned int i = 0; i < atms->size; i++)
                {
                        atms->elements[i] = new atmosphere_t();
                        file >> atms->elements[i];
                }
        }

        if (B_3D_raster_device)
                B_3D_raster_device->unknown084();

        for(unsigned int i = 0; i < atms->size; i++)
        {
                atmosphere_t * atm = (atmosphere_t *)atms->elements[i];
                B_3D_raster_device->add_atmosphere(atm->Id(), atm->color, atm->intensity);
        }

        read_points(&file, &gbl_world_points);
        read_sectors(&file, &world->sectors);

        array_t<light_t *> *lights = &world->lights;
        if (lights->num_alloc != 0)
        {
                for (unsigned int i = 0; i < lights->size; i++)
                {
                        light_t * light = (light_t *)lights->elements[i];
                        delete light;
                }
                delete(lights->elements);
                lights->elements = NULL;
                lights->size = 0;
                lights->num_alloc = 0;
        }

        file >> num_lights;

        for(unsigned int i = 0; i < num_lights; i++)
        {
                light_t *light = read_light(&file);
                array_t<light_t *> *lights = &world->lights;
                if (lights->num_alloc <= lights->size)
                {
                        lights->num_alloc += lights->increment;
                        if (lights->size != 0)
                        {
                                light_t **elements = new light_t *[lights->num_alloc];
                                for(unsigned int j = 0; j < lights->size; j++)
                                {
                                        elements[j] = lights->elements[j]; 
                                }
                                delete lights->elements;
                                lights->elements = elements;
                        }
                        else
                        {
                                lights->elements = new light_t *[lights->num_alloc];
                        }
                }
                lights->elements[i] = light;
                lights->size++;
        }

        for(unsigned int i = 0; i < num_lights; i++)
        {
                light_t *light = world->lights.elements[i];
                light->unknown008();
        }

        file >> world->initial_point_position >> world->initial_point_orientation;

        for(unsigned int i = 0; i < world->sectors.size; i++)
        {
                B_Sector *sector = world->sectors.elements[i];
                file >> sector->groupId;
        }

        world->unknown18F8.unknown_00451A21(&world->sectors, 0, 0x40B38800/*30000.0lf*/);

        return file;
}

/*
................................................................................
................................................................................
................................................................................
................................................................................
*/