// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Buffers;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Text;

namespace System
{
    public class UriBuilder
    {
        private string _scheme = "http";
        private string _username = string.Empty;
        private string _password = string.Empty;
        private string _host = "localhost";
        private int _port = -1;
        private string _path = "/";
        private string _query = string.Empty;
        private string _queryRfc3986 = string.Empty;
        private string _fragment = string.Empty;

        private bool _changed = true;
        private Uri? _uri;

        public UriBuilder()
        {
        }

        public UriBuilder([StringSyntax(StringSyntaxAttribute.Uri)] string uri)
        {
            // setting allowRelative=true for a string like www.acme.org
            _uri = new Uri(uri, UriKind.RelativeOrAbsolute);

            if (!_uri.IsAbsoluteUri)
            {
                _uri = new Uri(Uri.UriSchemeHttp + Uri.SchemeDelimiter + uri);
            }

            SetFieldsFromUri();
        }

        public UriBuilder(Uri uri)
        {
            ArgumentNullException.ThrowIfNull(uri);

            _uri = uri;
            SetFieldsFromUri();
        }

        public UriBuilder(string? schemeName, string? hostName)
        {
            Scheme = schemeName;
            Host = hostName;
        }

        public UriBuilder(string? scheme, string? host, int portNumber)
            : this(scheme, host)
        {
            Port = portNumber;
        }

        public UriBuilder(string? scheme, string? host, int port, string? pathValue)
            : this(scheme, host, port)
        {
            Path = pathValue;
        }

        public UriBuilder(string? scheme, string? host, int port, string? path, string? extraValue)
            : this(scheme, host, port, path)
        {
            if (!string.IsNullOrEmpty(extraValue))
            {
                if (extraValue[0] == '#')
                {
                    _fragment = extraValue;
                }
                else if (extraValue[0] == '?')
                {
                    int fragmentIndex = extraValue.IndexOf('#');
                    if (fragmentIndex == -1)
                    {
                        _query = extraValue;
                    }
                    else
                    {
                        _query = extraValue.Substring(0, fragmentIndex);
                        _fragment = extraValue.Substring(fragmentIndex);
                    }
                }
                else
                {
                    throw new ArgumentException(SR.Argument_ExtraNotValid, nameof(extraValue));
                }

                if (_query.Length == 1)
                {
                    _query = string.Empty;
                }

                if (_fragment.Length == 1)
                {
                    _fragment = string.Empty;
                }
            }
        }

        [AllowNull]
        public string Scheme
        {
            get => _scheme;
            set
            {
                value ??= string.Empty;

                if (value.Length != 0)
                {
                    if (!Uri.CheckSchemeName(value))
                    {
                        int index = value.IndexOf(':');
                        if (index != -1)
                        {
                            value = value.Substring(0, index);
                        }

                        if (!Uri.CheckSchemeName(value))
                        {
                            throw new ArgumentException(SR.net_uri_BadScheme, nameof(value));
                        }
                    }

                    value = value.ToLowerInvariant();
                }

                _scheme = value;
                _changed = true;
            }
        }

        [AllowNull]
        public string UserName
        {
            get => _username;
            set
            {
                _username = value ?? string.Empty;
                _changed = true;
            }
        }

        [AllowNull]
        public string Password
        {
            get => _password;
            set
            {
                _password = value ?? string.Empty;
                _changed = true;
            }
        }

        [AllowNull]
        public string Host
        {
            get => _host;
            set
            {
                if (!string.IsNullOrEmpty(value) && value.Contains(':') && value[0] != '[')
                {
                    //probable ipv6 address - Note: this is only supported for cases where the authority is inet-based.
                    value = "[" + value + "]";
                }

                _host = value ?? string.Empty;
                _changed = true;
            }
        }

        public int Port
        {
            get => _port;
            set
            {
                ArgumentOutOfRangeException.ThrowIfLessThan(value, -1);
                ArgumentOutOfRangeException.ThrowIfGreaterThan(value, 0xFFFF);
                _port = value;
                _changed = true;
            }
        }

        [AllowNull]
        public string Path
        {
            get => _path;
            set
            {
                _path = string.IsNullOrEmpty(value)
                    ? "/"
                    : Uri.InternalEscapeString(value.Replace('\\', '/'));
                _changed = true;
            }
        }

        [AllowNull]
        public string Query
        {
            get => _query;
            set
            {
                if (!string.IsNullOrEmpty(value) && value[0] != '?')
                {
                    value = '?' + value;
                }

                _query = value ?? string.Empty;
                _changed = true;
            }
        }

        [AllowNull]
        public string QueryRfc3986
        {
            get => _queryRfc3986;
            set
            {
                _queryRfc3986 = value ?? string.Empty;
                _changed = true;
            }
        }

        [AllowNull]
        public string Fragment
        {
            get => _fragment;
            set
            {
                if (!string.IsNullOrEmpty(value) && value[0] != '#')
                {
                    value = '#' + value;
                }

                _fragment = value ?? string.Empty;
                _changed = true;
            }
        }

        public Uri Uri
        {
            get
            {
                if (_changed)
                {
                    _uri = new Uri(ToString());
                    SetFieldsFromUri();
                    _changed = false;
                }
                else
                {
                    Debug.Assert(_uri is not null);
                }
                return _uri;
            }
        }

        public override bool Equals([NotNullWhen(true)] object? rparam) => rparam is not null && Uri.Equals(rparam.ToString());

        public override int GetHashCode() => Uri.GetHashCode();

        // The following characters ("/" / "\" / "?" / "#" / "@") are from the gen-delims group.
        // We have to escape them to avoid corrupting the rest of the Uri string.
        // Other characters like spaces or non-ASCII will be escaped by Uri, we can ignore them here.
        private static readonly IndexOfAnyValues<char> s_userInfoReservedChars =
            IndexOfAnyValues.Create(@"/\?#@");

        private static string EncodeUserInfo(string input)
        {
            if (input.AsSpan().IndexOfAny(s_userInfoReservedChars) < 0)
            {
                return input;
            }

            return input
                .Replace("/", "%2F", StringComparison.Ordinal)
                .Replace(@"\", "%5C", StringComparison.Ordinal)
                .Replace("?", "%3F", StringComparison.Ordinal)
                .Replace("#", "%23", StringComparison.Ordinal)
                .Replace("@", "%40", StringComparison.Ordinal);
        }

        private void SetFieldsFromUri()
        {
            Debug.Assert(_uri is not null);
            _scheme = _uri.Scheme;
            _host = _uri.Host;
            _port = _uri.Port;
            _path = _uri.AbsolutePath;
            _query = _uri.Query;
            _fragment = _uri.Fragment;

            string userInfo = _uri.UserInfo;

            if (userInfo.Length > 0)
            {
                int index = userInfo.IndexOf(':');

                if (index != -1)
                {
                    _password = userInfo.Substring(index + 1);
                    _username = userInfo.Substring(0, index);
                }
                else
                {
                    _username = userInfo;
                }
            }
        }

        public override string ToString()
        {
            if (UserName.Length == 0 && Password.Length != 0)
            {
                throw new UriFormatException(SR.net_uri_BadUserPassword);
            }

            var vsb = new ValueStringBuilder(stackalloc char[Uri.StackallocThreshold]);

            string scheme = Scheme;
            string host = Host;

            if (scheme.Length != 0)
            {
                UriParser? syntax = UriParser.GetSyntax(scheme);
                string schemeDelimiter;
                if (syntax is null)
                {
                    schemeDelimiter = host.Length == 0 ? ":" : Uri.SchemeDelimiter;
                }
                else
                {
                    schemeDelimiter = syntax.InFact(UriSyntaxFlags.MustHaveAuthority)
                        || (host.Length != 0 && syntax.NotAny(UriSyntaxFlags.MailToLikeUri) && syntax.InFact(UriSyntaxFlags.OptionalAuthority))
                            ? Uri.SchemeDelimiter
                            : ":";
                }

                vsb.Append(scheme);
                vsb.Append(schemeDelimiter);
            }

            string username = EncodeUserInfo(UserName);
            if (username.Length != 0)
            {
                vsb.Append(username);

                string password = EncodeUserInfo(Password);
                if (password.Length != 0)
                {
                    vsb.Append(':');
                    vsb.Append(password);
                }

                vsb.Append('@');
            }

            if (host.Length != 0)
            {
                vsb.Append(host);

                if (_port != -1)
                {
                    vsb.Append(':');

                    const int MaxUshortLength = 5;
                    bool success = _port.TryFormat(vsb.AppendSpan(MaxUshortLength), out int charsWritten);
                    Debug.Assert(success);
                    vsb.Length -= MaxUshortLength - charsWritten;
                }
            }

            var path = Path;
            if (path.Length != 0)
            {
                if (!path.StartsWith('/') && host.Length != 0)
                {
                    vsb.Append('/');
                }

                vsb.Append(path);
            }

            vsb.Append(Query);

            vsb.Append(Fragment);

            return vsb.ToString();
        }
    }
}
