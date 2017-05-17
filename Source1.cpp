
void main()
{

}
const char *generateFilename(const char *base, int number, const char *ext) {
	char  buf[256];
	sprintf(buf, "%s%d.%s", base, number, ext);
	return buf;
}

