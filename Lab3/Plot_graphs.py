import matplotlib.pyplot as plt
from google.colab import files

print("Загрузите measurements.csv:")
uploaded = files.upload()

filename = list(uploaded.keys())[0]
print(f"Загружен файл: {filename}")

with open(filename, 'r') as f:
    content = f.read()

data = {}
current = None

for line in content.split('\n'):
    line = line.strip()
    if not line:
        continue

    if ',' not in line and line.lower() not in ['n', 'build_ops', 'avg_query_ops']:
        current = line
        data[current] = {'n': [], 'build': [], 'query': []}
        print(f"Найдена структура: {current}")
        continue

    if 'n,build_ops' in line.lower():
        continue
    if ',' in line and current:
        parts = line.split(',')
        if len(parts) == 3:
            try:
                data[current]['n'].append(int(parts[0]))
                data[current]['build'].append(int(parts[1]))
                data[current]['query'].append(float(parts[2]))
            except:
                pass

print(f"\n ВСЕГО структур: {len(data)}")
for name, vals in data.items():
    print(f"  • {name}: {len(vals['n'])} точек")

if len(data) == 0:
    print("\n ОШИБКА: Данные не прочитаны!")
else:
    plt.figure(figsize=(12, 8))
    for name, vals in data.items():
        if vals['n']:
            plt.plot(vals['n'], vals['build'], 'o-', label=name, linewidth=2, markersize=6)

    plt.xlabel('n', fontsize=12)
    plt.ylabel('Операции построения', fontsize=12)
    plt.title('Сложность построения', fontsize=14, fontweight='bold')
    plt.xscale('log')
    plt.yscale('log')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('build.png', dpi=300)
    plt.show()

    plt.figure(figsize=(12, 8))
    for name, vals in data.items():
        if vals['n']:
            plt.plot(vals['n'], vals['query'], 's-', label=name, linewidth=2, markersize=6)

    plt.xlabel('n', fontsize=12)
    plt.ylabel('Операции на запрос', fontsize=12)
    plt.title('Сложность запросов', fontsize=14, fontweight='bold')
    plt.xscale('log')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('query.png', dpi=300)
    plt.show()

    files.download('build.png')
    files.download('query.png')
    print("\nГотово!")
