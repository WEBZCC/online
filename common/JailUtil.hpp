/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; fill-column: 100 -*- */
/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>

#include <Poco/File.h>
#include <Poco/Path.h>

namespace JailUtil
{
// The Child-Root directory structure looks like this:
// Ideally, we would have all the jails in a sub-dir.
//
// child-root/
// ├── <jail-id-1>
// ├── <jail-id-N>
// └── tmp
//     └── incoming

constexpr const char JAIL_SIGNAL_LOG_FILE_PATH[] = "/tmp/signal.log";

/// General temporary directory owned by us.
constexpr const char CHILDROOT_TMP_DIR[] = "tmp";

/// Files uploaded by users are stored in this sub-directory of child-root.
constexpr const char CHILDROOT_TMP_INCOMING_PATH[] = "/tmp/incoming";

/// The LO installation directory with jail.
constexpr const char LO_JAIL_SUBPATH[] = "lo";

/// Returns the jails path within the Child-Root.
/// Child-Root/
inline std::string getJailsPath(const std::string& childRoot)
{
    return Poco::Path::forDirectory(childRoot).toString();
}

/// Returns a jail's path, given it's Jail-ID.
/// Child-Root/<jailId>
inline std::string getJailPath(const std::string& childRoot, const std::string& jailId)
{
    return Poco::Path::forDirectory(getJailsPath(childRoot)).pushDirectory(jailId).toString();
}

/// Returns the temp directory path in the Child-Root.
/// Child-Root/Tmp
inline std::string getChildRootTmpPath(const std::string& childRoot)
{
    return Poco::Path::forDirectory(childRoot).pushDirectory(CHILDROOT_TMP_DIR).toString();
}

/// Returns the temp incoming-files directory path in the Child-Root.
/// Child-Root/Tmp/Incoming
inline std::string getChildRootTmpIncomingPath(const std::string& childRoot)
{
    return Poco::Path::forDirectory(childRoot)
        .pushDirectory(CHILDROOT_TMP_INCOMING_PATH)
        .toString();
}

/// Returns the LO path for the given root path.
/// Root/Lo
inline std::string getLoPath(const std::string& root)
{
    return Poco::Path::forDirectory(root).pushDirectory(LO_JAIL_SUBPATH).toString();
}

/// Returns the full path to the jailed signalLog file.
/// Root/tmp/signal.log
inline std::string getJailedSignalLogFilePath(const std::string& root, const std::string& domain)
{
    const std::string filename = std::string(JAIL_SIGNAL_LOG_FILE_PATH) + domain;
    return Poco::Path::forDirectory(root).setFileName(filename).toString();
}

/// Returns the temp directory path in the Child-Root.
/// Child-Root/<jail-id>/tmp/signal.log<domain>
inline std::string getChildRootSignalLogFilePath(const std::string& childRoot,
                                                 const std::string& jailId,
                                                 const std::string& domain)
{
    return getJailedSignalLogFilePath(getJailPath(childRoot, jailId), domain);
}

/// Bind mount a jail directory.
bool bind(const std::string& source, const std::string& target);

/// Remount a bound mount point as readonly.
bool remountReadonly(const std::string& source, const std::string& target);

/// Marks a jail as having been copied instead of mounted.
void markJailCopied(const std::string& root);

/// Returns true iff the jail in question was copied and not mounted.
bool isJailCopied(const std::string& root);

/// Remove the jail directory and all its contents.
void removeJail(const std::string& root);

/// Remove all jails.
void cleanupJails(const std::string& jailRoot);

/// Setup the Child-Root directory.
void setupChildRoot(bool bindMount, const std::string& jailRoot, const std::string& sysTemplate);

/// Setup /dev/random and /dev/urandom in the given jail path.
void setupJailDevNodes(const std::string& root);

/// Enable bind-mounting in this process.
void enableBindMounting();

/// Disable bind-mounting in this process.
void disableBindMounting();

/// Returns true iff bind-mounting is enabled in this process.
bool isBindMountingEnabled();

namespace SysTemplate
{
/// Setup links for /dev/random and /dev/urandom in systemplate.
void setupRandomDeviceLinks(const std::string& root);

/// Setup the dynamic files within the sysTemplate by either
/// copying or linking. See updateJail_DynamicFilesInSysTemplate.
/// If the dynamic files need updating and systemplate is read-only,
/// this will fail and mark files for copying.
void setupDynamicFiles(const std::string& sysTemplate);

/// Update the dynamic files within the sysTemplate before each child fork.
/// Returns false on failure.
bool updateDynamicFiles(const std::string& sysTemplate);

} // namespace SysTemplate

} // end namespace JailUtil

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
