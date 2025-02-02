/*
******************************************************************************
* Copyright (C) 2010, International Business Machines Corporation and        *
* others. All Rights Reserved.                                               *
******************************************************************************
*/

#ifndef LOCDSPNM_H
#define LOCDSPNM_H

#include "third_party/icu/utypes.h"

/**
 * \file
 * \brief C++ API: Provides display names of Locale and its components.
 */

#if !UCONFIG_NO_FORMATTING

#include "third_party/icu/locid.h"
#include "third_party/icu/uscript.h"
#include "third_party/icu/uldnames.h"

U_NAMESPACE_BEGIN

/**
 * Returns display names of Locales and components of Locales. For
 * more information on language, script, region, variant, key, and
 * values, see Locale.
 * @draft ICU 4.4
 */
class U_I18N_API LocaleDisplayNames : public UObject {
public:
    /**
     * Destructor.
     * @draft ICU 4.4
     */
    virtual ~LocaleDisplayNames();

    /**
     * Convenience overload of
     * {@link #createInstance(const Locale& locale, UDialectHandling dialectHandling)}
     * that specifies STANDARD dialect handling.
     * @param locale the display locale
     * @return a LocaleDisplayNames instance
     * @draft ICU 4.4
     */
    static LocaleDisplayNames* U_EXPORT2 createInstance(const Locale& locale);

    /**
     * Returns an instance of LocaleDisplayNames that returns names
     * formatted for the provided locale, using the provided
     * dialectHandling.
     *
     * @param locale the display locale
     * @param dialectHandling how to select names for locales
     * @return a LocaleDisplayNames instance
     * @draft ICU 4.4
     */
    static LocaleDisplayNames* U_EXPORT2 createInstance(const Locale& locale,
							UDialectHandling dialectHandling);

    // getters for state
    /**
     * Returns the locale used to determine the display names. This is
     * not necessarily the same locale passed to {@link #getInstance}.
     * @return the display locale
     * @draft ICU 4.4
     */
    virtual const Locale& getLocale() const = 0;

    /**
     * Returns the dialect handling used in the display names.
     * @return the dialect handling enum
     * @draft ICU 4.4
     */
    virtual UDialectHandling getDialectHandling() const = 0;

    // names for entire locales
    /**
     * Returns the display name of the provided locale.
     * @param locale the locale whose display name to return
     * @param result receives the locale's display name
     * @return the display name of the provided locale
     * @draft ICU 4.4
     */
    virtual UnicodeString& localeDisplayName(const Locale& locale,
					     UnicodeString& result) const = 0;

    /**
     * Returns the display name of the provided locale id.
     * @param localeId the id of the locale whose display name to return
     * @param result receives the locale's display name
     * @return the display name of the provided locale
     * @draft ICU 4.4
     */
    virtual UnicodeString& localeDisplayName(const char* localeId,
					     UnicodeString& result) const = 0;

    // names for components of a locale id
    /**
     * Returns the display name of the provided language code.
     * @param lang the language code
     * @param result receives the language code's display name
     * @return the display name of the provided language code
     * @draft ICU 4.4
     */
    virtual UnicodeString& languageDisplayName(const char* lang,
					       UnicodeString& result) const = 0;

    /**
     * Returns the display name of the provided script code.
     * @param script the script code
     * @param result receives the script code's display name
     * @return the display name of the provided script code
     * @draft ICU 4.4
     */
    virtual UnicodeString& scriptDisplayName(const char* script,
					     UnicodeString& result) const = 0;

    /**
     * Returns the display name of the provided script code.
     * @param scriptCode the script code number
     * @param result receives the script code's display name
     * @return the display name of the provided script code
     * @draft ICU 4.4
     */
    virtual UnicodeString& scriptDisplayName(UScriptCode scriptCode,
					     UnicodeString& result) const = 0;

    /**
     * Returns the display name of the provided region code.
     * @param region the region code
     * @param result receives the region code's display name
     * @return the display name of the provided region code
     * @draft ICU 4.4
     */
    virtual UnicodeString& regionDisplayName(const char* region,
					     UnicodeString& result) const = 0;

    /**
     * Returns the display name of the provided variant.
     * @param variant the variant string
     * @param result receives the variant's display name
     * @return the display name of the provided variant
     * @draft ICU 4.4
     */
    virtual UnicodeString& variantDisplayName(const char* variant,
					      UnicodeString& result) const = 0;

    /**
     * Returns the display name of the provided locale key.
     * @param key the locale key name
     * @param result receives the locale key's display name
     * @return the display name of the provided locale key
     * @draft ICU 4.4
     */
    virtual UnicodeString& keyDisplayName(const char* key,
					  UnicodeString& result) const = 0;

    /**
     * Returns the display name of the provided value (used with the provided key).
     * @param key the locale key name
     * @param value the locale key's value
     * @param result receives the value's display name
     * @return the display name of the provided value
     * @draft ICU 4.4
     */
    virtual UnicodeString& keyValueDisplayName(const char* key, const char* value,
					       UnicodeString& result) const = 0;

    /**
     * ICU "poor man's RTTI", returns a UClassID for this class.
     * @returns a UClassID for this class.
     * @internal ICU 4.4  // TODO @draft ICU 4.6
     */
    static UClassID U_EXPORT2 getStaticClassID();
};

inline LocaleDisplayNames::~LocaleDisplayNames() {
}

inline LocaleDisplayNames* LocaleDisplayNames::createInstance(const Locale& locale) {
  return LocaleDisplayNames::createInstance(locale, ULDN_STANDARD_NAMES);
}

U_NAMESPACE_END

#endif

#endif
