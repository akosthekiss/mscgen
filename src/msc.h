/***************************************************************************
 *
 * $Id$
 *
 * The message sequence parser API.
 * Copyright (C) 2009 Michael C McTernan, Michael.McTernan.2001@cs.bris.ac.uk
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 ***************************************************************************/

#ifndef MSC_H
#define MSC_H

#include <stdbool.h>

/***************************************************************************
 * Types
 ***************************************************************************/

/** Msc Options.
 */
typedef enum MscOptTypeTag
{
    MSC_OPT_HSCALE,
    MSC_OPT_WIDTH,
    MSC_OPT_ARCGRADIENT,
    MSC_OPT_WORDWRAPARCS
}
MscOptType;


/** Arc attributes.
 * An arc may have one or more attributes listed in square brackets after
 * the declaration.  This gives an enumerated type for each permissible
 * attribute.
 */
typedef enum MscAttribTypeTag
{
    MSC_ATTR_LABEL,
    MSC_ATTR_ID,
    MSC_ATTR_URL,
    MSC_ATTR_IDURL,
    MSC_ATTR_LINE_COLOUR,
    MSC_ATTR_TEXT_COLOUR,
    MSC_ATTR_TEXT_BGCOLOUR,
    MSC_ATTR_ARC_LINE_COLOUR,
    MSC_ATTR_ARC_TEXT_COLOUR,
    MSC_ATTR_ARC_TEXT_BGCOLOUR,
    MSC_ATTR_NO_ARROWS,
    MSC_ATTR_BI_ARROWS,
    MSC_ATTR_ARC_SKIP
}
MscAttribType;


typedef enum
{
    MSC_ARC_METHOD,
    MSC_ARC_RETVAL,
    MSC_ARC_SIGNAL,
    MSC_ARC_CALLBACK,
    MSC_ARC_DOUBLE,
    MSC_ARC_DISCO,      /* ... Discontinuity in time line */
    MSC_ARC_DIVIDER,    /* --- Divider */
    MSC_ARC_SPACE,      /* ||| */
    MSC_ARC_PARALLEL,   /* Comma instead of semicolon */
    MSC_ARC_BOX,
    MSC_ARC_ABOX,
    MSC_ARC_RBOX,
    MSC_ARC_NOTE,
    MSC_ARC_LOSS,       /* -x or x- */
    MSC_ARC_ACT,
    MSC_ARC_DEACT,
    MSC_ARC_DESTR,

    MSC_INVALID_ARC_TYPE
}
MscArcType;


/***************************************************************************
 * Abstract types
 ***************************************************************************/

typedef struct MscTag           *Msc;

typedef struct MscOptTag        *MscOpt;

typedef struct MscEntityTag     *MscEntity;

typedef struct MscEntityListTag *MscEntityList;

typedef struct MscArcTag        *MscArc;

typedef struct MscArcListTag    *MscArcList;

typedef struct MscAttribTag     *MscAttrib;


/***************************************************************************
 * Iterator types
 ***************************************************************************/

typedef struct
{
    MscEntity entity;
}
MscEntityIter;

typedef struct
{
    MscArc arc;
}
MscArcIter;

/***************************************************************************
 * MSC Building Functions
 ***************************************************************************/

/** Parse some input to build a message sequence chart.
 * This will parse characters from \a in and build a message sequence chart
 * ADT.
 * \retval Msc  The message sequence chart, which may equal \a NULL is a
 *               parse error occurred.
 */
Msc           MscParse(FILE *in);

MscEntity     MscAllocEntity(char *entityName);

MscEntityList MscLinkEntity(MscEntityList list, MscEntity elem);

void          MscPrintEntityList(MscEntityList list);

MscOpt        MscAllocOpt(MscOptType  type,
                          char       *value);

MscOpt        MscLinkOpt(MscOpt head,
                         MscOpt newHead);

MscArc        MscAllocArc(char        *srcEntity,
                          char        *dstEntity,
                          MscArcType   type,
                          unsigned int inputLine);

MscArcList    MscLinkArc (MscArcList list,
                          MscArc     elem);

void          MscPrintArcList(struct MscArcListTag *list);

MscAttrib     MscAllocAttrib(MscAttribType  type,
                             char          *value);

MscAttrib     MscLinkAttrib(MscAttrib head,
                            MscAttrib newHead);

void          MscArcLinkAttrib(MscArc    arc,
                               MscAttrib att);

void          MscEntityLinkAttrib(MscEntity ent,
                                  MscAttrib att);

void          MscPrintAttrib(const struct MscAttribTag *att);

const char   *MscPrettyAttribType(MscAttribType t);

Msc           MscAlloc(MscOpt        optList,
                       MscEntityList entityList,
                       MscArcList    arcList);

void          MscFree(struct MscTag *m);

/** Print the passed msc in textual form to stdout.
 * This prints a human readable format of the parsed msc to stdout.  This
 * is primarily of use in debugging the parser.
 */
void          MscPrint(Msc m);

unsigned int  MscGetNumEntities(Msc m);

unsigned int  MscGetNumArcs(Msc m);

unsigned int  MscGetNumParallelArcs(Msc m);

unsigned int  MscGetNumOpts(Msc m);

/** Get an MSC option, returning the value as a float.
 *
 * \param[in]     m      The MSC to analyse.
 * \param[in]     type   The option type to retrieve.
 * \param[in,out] f      Pointer to be filled with parsed value.
 * \retval true  If the option was found and parsed successfully.
 */
bool         MscGetOptAsFloat(struct MscTag *m, MscOptType type, float *const f);

/** Get an MSC option, returning the value as a bool.
 *
 * \param[in]     m      The MSC to analyse.
 * \param[in]     type   The option type to retrieve.
 * \param[in,out] b      Pointer to be filled with parsed value.
 * \retval true  If the option was found and parsed successfully.
 * \retval false On error, in which case *b is unmodified.
 *
 */
bool         MscGetOptAsBoolean(struct MscTag *m, MscOptType type, bool *const b);

/** Get the index of some entity.
 * This returns the column index for the entity identified by the passed
 * label.
 *
 * \param  m      The MSC to analyse.
 * \param  label  The label to find.
 * \retval -1     If the label was not found, otherwise the columnn index.
 */
int           MscGetEntityIndex(struct MscTag *m, const char *label);

/***************************************************************************
 * Entity processing functions
 ***************************************************************************/

/** \defgroup EntityFuncs  Entity handling functions
 * @{
 */

/** Returns an entity iterator.
 * The current entity of the iterator will be the head of the list.
 */
MscEntityIter  MscEntityIterBegin(Msc m);

/** Checks whether the iterator moved along the list.
 * \retval true if the end of the list has been reached.
 * \retval false if the current entity is valid.
 */
bool           MscEntityIterEnd(MscEntityIter *i);

/** Move to the next entity in the MSC.
 */
void           MscNextEntity(MscEntityIter *i);

/** Get the value of some attribute for the current entity.
 * \retval The attribute string, or NULL if unset.
 */
const char    *MscGetEntAttrib(MscEntityIter *i, MscAttribType a);

/** Get an attribute associated with some entity.
 * \param[in] entIdx  The index of the entity.
 * \retval The attribute string, or NULL if unset.
 */
const char    *MscGetEntIdxAttrib(Msc m, unsigned int entIdx, MscAttribType a);

/** @} */

/***************************************************************************
 * Arc processing functions
 ***************************************************************************/

/** \defgroup ArcFuncs  Arc handling functions
 * @{
 */

/** Returns an arc iterator.
 * The current arc of the iterator will be the head of the list.
 */
MscArcIter   MscArcIterBegin(Msc m);

/** Checks whether the iterator moved along the list.
 * \retval true if the end of the list has been reached.
 * \retval false if the current arc is valid.
 */
bool         MscArcIterEnd(MscArcIter *i);

/** Move to the next arc in the MSC.
 * \retval true if there is another arc.
 * \retval false if the end of the list has been reached.
 */
void         MscNextArc(MscArcIter *i);

/** Get the name of the entity from which the current arc originates.
 * \returns The label for the entity from which the current arc starts.
 *           The returned string must not be modified.
 */
const char  *MscGetArcSource(MscArcIter *i);

/** Get the name of the entity at which the current arc terminates.
 * \retval The label for the entity at which the current arc stops.
 *          The returned string must not be modified.
 */
const char  *MscGetArcDest(MscArcIter *i);

/** Get the type for some arc.
 *
 */
MscArcType   MscGetArcType(MscArcIter *i);

/** Get the value of some attribute for the current arc.
 * \returns The attribute string, or NULL if unset.
 */
const char  *MscGetArcAttrib(MscArcIter *i, MscAttribType a);

/** Get the line of the input file at which the current arc was defined.
 * \returns The line number of the input file.
 */
unsigned int MscGetArcInputLine(MscArcIter *i);

/** @} */

#endif /* MSC_H */

/* END OF FILE */
